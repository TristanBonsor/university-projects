/*******************************************************************************
 * File:   main.c
 *
 * Copyright (C) 2012, 2013  Tristan W. Bonsor
 *
 * This work is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 2.
 *
 * Description: Basic driver program for testing the virtual hard disk module.
 ******************************************************************************/
#include <sys/stat.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "vhd.h"

#define MAX_DRIVE_NAME 16
#define MAX_FILE_NAME  16
#define MAX_PATH_NAME 255

vhd* current_disk = 0;

int create_disk();
int mount_disk();
int write_to_disk();
int read_from_disk();
int remove_from_disk();
int unmount_disk();
void print_menu();

int main()
{
    
    char choice;
    int run, error;

    printf("************************************************\n");
    printf("           Virtual Disk Manager\n");
    printf("************************************************\n");

    print_menu();

    run = 1;
    while(run){
        printf("\n>");
        scanf("%c", &choice);
        
        switch(choice){
        case '1':
            error = create_disk();
            break;
        case '2':
            error = mount_disk();
            break;
        case '3':
            error = write_to_disk();
            printf("%d bytes written\n", error);
            break;
        case '4':
            error = read_from_disk();
            break;
        case '5':
            error = remove_from_disk();
            break;
        case '6':
            error = unmount_disk();
            break;
        case 'h':
        case 'H':
            print_menu();
            break;
        case 'q':
        case 'Q':
            run = 0;
            break;
        }
        while(getc(stdin) != '\n');
    }
    return 0;
}

int create_disk()
{
    int i;
    size_t disk_size, block_size;
    char disk_name[MAX_DRIVE_NAME];
    for(i = 0; i < MAX_DRIVE_NAME; ++i){
        disk_name[i] = '\0';
    }
    printf("Enter name of virtual disk:\n>");
    scanf("%s", disk_name);
    disk_name[MAX_DRIVE_NAME - 1] = '\0';
    printf("Enter size of virtual disk (bytes):\n>");
    scanf("%zu", &disk_size);
    printf("Enter block size of virtual disk (bytes):\n>");
    scanf("%zu", &block_size);
    return vhd_create(disk_name, disk_size, block_size);
}

int mount_disk()
{
    int i;
    char disk_name[MAX_DRIVE_NAME];
    for(i = 0; i < MAX_DRIVE_NAME; ++i){
        disk_name[i] = '\0';
    }
    if(!current_disk){
        printf("Enter name of virtual disk:\n>");
        scanf("%s", disk_name);
        disk_name[MAX_DRIVE_NAME - 1] = '\0';
        current_disk = vhd_mount(disk_name);
        if(!current_disk)
            return 0;
        return 1;
    }
    printf("A disk is already mounted; please unmount it first.\n");
    return 0;
}

int write_to_disk()
{
    int fd, i;
    unsigned char byte_buf;
    char path_name[MAX_PATH_NAME];
    char file_name[MAX_FILE_NAME];
    size_t file_size, nbyte, vfd;
    struct stat st;

    if(!current_disk){
        printf("No virtual disk is currently mounted; please mount a disk.\n");
        return 0;
    }

    for(i = 0; i < MAX_PATH_NAME; ++i){
        path_name[i] = '\0';
    }

    for(i = 0; i < MAX_FILE_NAME; ++i){
        file_name[i] = '\0';
    }

    printf("Enter name of file to write from:\n>");
    scanf("%s", path_name);
    printf("Enter name of file to write to:\n>");
    scanf("%s", file_name);

    stat(path_name, &st);
    file_size = st.st_size;

    fd = open(path_name, O_RDONLY);
    vfd = vhd_open_file(current_disk, file_name);

    printf("Writing file to virtual disk...\n");

    for(nbyte = 0; nbyte < file_size; ++nbyte){
        pread(fd, &byte_buf, sizeof(unsigned char), nbyte);
        vhd_write_file(current_disk, vfd, &byte_buf, sizeof(unsigned char), nbyte);
    }
    close(fd);
    vhd_close_file(current_disk, vfd);
    return nbyte;
}

int read_from_disk()
{
    int fd, i;
    unsigned char byte_buf;
    char path_name[MAX_PATH_NAME];
    char file_name[MAX_FILE_NAME];
    size_t file_size, nbyte, vfd;
    mode_t mode;
    struct stat st;

    if(!current_disk){
        printf("No virtual disk is currently mounted; please mount a disk.\n");
        return 0;
    }

    for(i = 0; i < MAX_PATH_NAME; ++i){
        path_name[i] = '\0';
    }

    for(i = 0; i < MAX_FILE_NAME; ++i){
        file_name[i] = '\0';
    }

    printf("Enter name of file to write to:\n>");
    scanf("%s", path_name);
    printf("Enter name of file to write from:\n>");
    scanf("%s", file_name);

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fd = open(path_name, O_WRONLY | O_CREAT, mode);
    vfd = vhd_open_file(current_disk, file_name);

    file_size = vhd_file_size(current_disk, file_name);

    printf("Writing file from virtual disk...\n");

    for(nbyte = 0; nbyte < file_size; ++nbyte){
        vhd_read_file(current_disk, vfd, &byte_buf, sizeof(unsigned char), nbyte);
        pwrite(fd, &byte_buf, sizeof(unsigned char), nbyte);
    }
    close(fd);
    vhd_close_file(current_disk, vfd);
    return 1;
}

int remove_from_disk()
{
    int i;
    char file_name[MAX_FILE_NAME];

    if(!current_disk){
        printf("No virtual disk is currently mounted; please mount a disk.\n");
        return 0;
    }

    for(i = 0; i < MAX_FILE_NAME; ++i){
        file_name[i] = '\0';
    }

    printf("Enter name of file to remove:\n>");
    scanf("%s", file_name);

    vhd_remove_file(current_disk, file_name);
    return 1;
}

int unmount_disk()
{
    if(!current_disk){
        printf("No virtual disk is currently mounted.\n");
        return 0;
    }
    vhd_unmount(&current_disk);
    return 1;
}

void print_menu()
{
    printf("\nSelect option:\n");
    printf("\t1 - Create a virtual disk\n");
    printf("\t2 - Mount a virtual disk\n");
    printf("\t3 - Write a file to virtual disk\n");
    printf("\t4 - Read a file from a virtual disk\n");
    printf("\t5 - Remove a file from a virtual disk\n");
    printf("\t6 - Unmount a virtual disk\n");
    printf("\th - Print this menu\n");
    printf("\tq - Quit\n");
}
