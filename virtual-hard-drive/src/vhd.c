/*******************************************************************************
 * File:    vhd.c
 *
 * Copyright (C) 2012, 2013  Tristan W. Bonsor
 *
 * This work is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 2.
 *
 * Description: Definition of virtual hard drive functions and data structures.
 ******************************************************************************/
#define _XOPEN_SOURCE 500

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "vhd.h"

/* Virtual hard drive minimum and maximum size parameters. */

#define MIN_DRIVE_SZ 1048576              /*   1 MB. */
#define MAX_DRIVE_SZ 134217728            /* 128 MB. */

#define MIN_BLOCK_SZ 512                  /* Bytes. */
#define MAX_BLOCK_SZ 8192                 /* Bytes. */

#define MIN_BLOCK_NUM 8
#define MAX_BLOCK_NUM 262144

#define INODE_SZ 96          /* Bytes. */
#define AGG_INODE_SZ 4768    /* Bytes. */
#define INDIRECT_BLOCK_SZ 64 /* Bytes. */

#define INODE_POINT_SZ 8
#define MAX_FILE_BLOCKS 592

#define MAX_FILE_NAME 16 /* Bytes. */

/* I-node data structure. Approx 96 byte file size (assuming size_t is 8
 * bytes.) */
typedef struct inode inode;
struct inode{
    size_t bytes;
    size_t direct[INODE_POINT_SZ];
    size_t s_indirect, d_indirect, t_indirect;
};

/* Opaque virtual hard drive data structure. */
struct vhd{
    int fd;
    size_t num_blocks, block_size, fsm_address, inode_address, fs_address;
    size_t num_inodes, num_files, num_open_files;
    inode* inodes;
    size_t* open_files, *inodes_avail;
};

/*******************************************************************************
 * Static function declarations.
 ******************************************************************************/
static void insert_in_order(size_t element,
                            size_t* array,
                            size_t num_elements,
                            size_t capacity);

static size_t binary_search(size_t element,
                            size_t* array,
                            size_t low,
                            size_t high);

static void remove_in_order(size_t element, size_t* array, size_t num_elements);

/* Packs value trimmed to bit_count wide, into old_byte at bit_offset. */
static unsigned char byte_pack(unsigned char old_byte,
                               unsigned char bit_offset,
                               unsigned char bit_count,
                               unsigned char value);

static void get_block(int fd,
                      unsigned char* buffer,
                      size_t number,
                      size_t block_byte);

static void set_block(int fd,
                      unsigned char* buffer,
                      size_t number,
                      size_t block_byte);

static void set_block_use(int fd,
                          unsigned char value,
                          size_t block_number,
                          size_t fsm_address,
                          size_t block_byte);

static int get_block_use(int fd,
                         size_t block_number,
                         size_t fsm_address,
                         size_t block_byte);

static void get_inode(int fd, inode* i_node, size_t number, size_t block_byte);

static void set_inode(int fd, inode* i_node, size_t number, size_t block_byte);

static void get_indirect_block(int fd,
                               size_t* iblock,
                               size_t number,
                               size_t block_byte,
                               size_t num_inodes);

static void set_indirect_block(int fd,
                               size_t* iblock,
                               size_t number,
                               size_t block_byte,
                               size_t num_inodes);

static void sanitize(size_t* disk_byte,
                     size_t* block_byte,
                     size_t* num_blocks,
                     size_t* bit_map_blocks,
                     size_t* inode_blocks);

/*******************************************************************************
 * Function definitions (declarations in vhd.h).
 ******************************************************************************/
size_t vhd_create(const char* drive_name, size_t disk_byte, size_t block_byte)
{
    unsigned char byte;
    int fd;
    size_t bit_map_blocks, num_blocks, inode_blocks, offset, ix, iy, iz, iw, iu;
    size_t nbytes, mbits, num_inodes;
    size_t s_indirect, d_indirect, t_indirect;
    inode i_node;
    unsigned char* block;
    size_t indirect_block[INODE_POINT_SZ], d_indirect_block[INODE_POINT_SZ],
        t_indirect_block[INODE_POINT_SZ];
    void* vp;

    /* Sanitize disk size and block size to prevent extra junk block. */
    sanitize(&disk_byte, &block_byte, &num_blocks, &bit_map_blocks, &inode_blocks);

    /* Based on block coverage of an i-node; allow for root dir listing and one
     * other file at least. */
    if(num_blocks < MAX_FILE_BLOCKS * 2)
        return 0;

    fd = open(drive_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    if(fd == -1)
        return 0;

    if(ftruncate(fd, disk_byte)){
        unlink(drive_name);
        return 0;
    }

    /* Write super block data */
    vp = &num_blocks;
    pwrite(fd, vp, sizeof(size_t), 0); /* Number of blocks. */

    vp = &block_byte;
    pwrite(fd, vp, sizeof(size_t), sizeof(size_t)); /* Block size. */

    offset = 1;
    vp = &offset;
    pwrite(fd, vp, sizeof(size_t), 2 * sizeof(size_t)); /* I-node start address. */

    offset += inode_blocks;
    vp = &offset;
    pwrite(fd, vp, sizeof(size_t), 3 * sizeof(size_t)); /* FSM start address.*/

    /* Write free space management bit map. */
    byte = 0x00;
    vp = &byte;
    /* Record superblock and i-node block coverage. */
    nbytes = (inode_blocks + 1) / 8;
    mbits = (inode_blocks + 1) % 8;
    for(ix = 0; ix < nbytes; ++ix){
        pwrite(fd, vp, sizeof(char), (inode_blocks + 1) * block_byte + ix);
    }
    /* Pack remainder in with fsm block coverage. */
    mbits += bit_map_blocks;
    nbytes = mbits / 8;
    mbits = mbits % 8;
    offset = (inode_blocks + 1) * block_byte + ix;
    for(ix = 0; ix < nbytes; ++ix){
        pwrite(fd, vp, sizeof(char), offset + ix);
    }
    /* Any remaining bits that should be included with the above coverages
     * are packed in with the beginning of the file system block coverage
     * part of the bit map.*/
    byte = byte_pack(0x00, 0, 8 - mbits, 0xff);
    vp = &byte;
    pwrite(fd, vp, sizeof(char), offset + ix++);
    /* Fill out rest of bit map as free. */
    byte = 0xff;
    vp = &byte;
    nbytes = ((num_blocks-(inode_blocks+1+bit_map_blocks)-(8-mbits))/8)+ 1;
    for(; ix < nbytes; ++ix){
        pwrite(fd, vp, sizeof(char), offset + ix);
    }

    /* Write i-node data. */
    num_inodes = inode_blocks * block_byte / AGG_INODE_SZ;
    i_node.bytes = 0;
    for(ix = 0; ix < INODE_POINT_SZ; ++ix){
        i_node.direct[ix] = (size_t)-1;
    }
    /* Set up single, double, and triple indirect pointers. */
    iu = ix = s_indirect = d_indirect = t_indirect = 0;
    for(nbytes = 0; nbytes < num_inodes * INODE_SZ; nbytes += INODE_SZ){
        i_node.s_indirect = s_indirect;
        i_node.d_indirect = d_indirect + 1;
        i_node.t_indirect = t_indirect + 2;
        /* Single indirect block. */
        get_indirect_block(fd, indirect_block, i_node.s_indirect, block_byte,
                           num_inodes);
        for(ix = 0; ix < INODE_POINT_SZ; ++ix){
            indirect_block[ix] = (size_t)-1;
        }
        set_indirect_block(fd, indirect_block, i_node.s_indirect, block_byte,
                           num_inodes);
        /* Double indirect block. */
        get_indirect_block(fd, indirect_block, i_node.d_indirect, block_byte,
                           num_inodes);
        iy = i_node.t_indirect;
        for(ix = 0; ix < INODE_POINT_SZ; ++ix){
            indirect_block[ix] = ++iy;
            get_indirect_block(fd, d_indirect_block, indirect_block[ix],
                               block_byte, num_inodes);
            for(iz = 0; iz < INODE_POINT_SZ; ++iz){
                d_indirect_block[iz] = (size_t)-1;
            }
            set_indirect_block(fd, d_indirect_block, indirect_block[ix],
                               block_byte, num_inodes);
        }
        set_indirect_block(fd, indirect_block, i_node.d_indirect, block_byte,
                           num_inodes);
        /* Triple indirect block. */
        get_indirect_block(fd, indirect_block, i_node.t_indirect, block_byte,
                           num_inodes);
        for(ix = 0; ix < INODE_POINT_SZ; ++ix){
            indirect_block[ix] = ++iy;
        }
        set_indirect_block(fd, indirect_block, i_node.t_indirect, block_byte,
                           num_inodes);
        for(ix = 0; ix < INODE_POINT_SZ; ++ix){
            get_indirect_block(fd, d_indirect_block, indirect_block[ix],
                               block_byte, num_inodes);
            for(iz = 0; iz < INODE_POINT_SZ; ++iz){
                d_indirect_block[iz] = ++iy;
                get_indirect_block(fd, t_indirect_block, d_indirect_block[iz],
                                   block_byte, num_inodes);
                for(iw = 0; iw < INODE_POINT_SZ; ++iw){
                    t_indirect_block[iw] = (size_t)-1;
                }
                set_indirect_block(fd, t_indirect_block, d_indirect_block[ix],
                                   block_byte, num_inodes);
            }
            set_indirect_block(fd, d_indirect_block, indirect_block[ix],
                               block_byte, num_inodes);
        }
        s_indirect = d_indirect = t_indirect = ++iy;
        set_inode(fd, &i_node, iu++, block_byte);
    }

    /* Create root i-node data.*/
    get_inode(fd, &i_node, 0, block_byte);
    i_node.bytes = MAX_FILE_NAME + sizeof(size_t); /* Root name and i-node addr. */
    i_node.direct[0] = inode_blocks + 1 + bit_map_blocks; /* First block of fs. */
    set_inode(fd, &i_node, 0, block_byte);
    /* Write root directory data. */
    block = (unsigned char*)calloc(block_byte, sizeof(unsigned char));
    block[8] = '/';
    set_block(fd, block, inode_blocks + 1 + bit_map_blocks, block_byte);
    free(block);

    /* Update free space management bit map. */
    set_block_use(fd, 0, inode_blocks + 1 + bit_map_blocks, inode_blocks + 1,
                  block_byte);

    close(fd);

    return 1;  /* TODO: Return true size of drive. */
}

vhd* vhd_mount(const char* drive_name)
{
    size_t bm_blocks, inode_number, i, ix;

    int fd = open(drive_name, O_RDWR, S_IRUSR | S_IWUSR);
    vhd* vdrive = (vhd*)malloc(sizeof(vhd));
    void* bf = malloc(sizeof(size_t));

    if(fd == -1){
        free(vdrive);
        free(bf);
        return 0;
    }

    vdrive->fd = fd;

    pread(vdrive->fd, bf, sizeof(size_t), 0);
    vdrive->num_blocks = *((size_t*)bf);

    pread(vdrive->fd, bf, sizeof(size_t), sizeof(size_t));
    vdrive->block_size = *((size_t*)bf);

    pread(vdrive->fd, bf, sizeof(size_t), 2 * sizeof(size_t));
    vdrive->inode_address = *((size_t*)bf);

    pread(vdrive->fd, bf, sizeof(size_t), 3 * sizeof(size_t));
    vdrive->fsm_address = *((size_t*)bf);

    bm_blocks = vdrive->num_blocks;
    for(; bm_blocks < (size_t)-1; bm_blocks++){
        if(bm_blocks % vdrive->block_size == 0){
            if((bm_blocks / vdrive->block_size) % 8 == 0){
                bm_blocks = ((bm_blocks / vdrive->block_size) / 8);
            }
            else{
                bm_blocks = ((bm_blocks / vdrive->block_size) / 8) + 1;
            }
            vdrive->fs_address = vdrive->fsm_address + bm_blocks;
            break;
        }
    }

    /* Assumes i-nodes begin at block 1. */
    vdrive->num_inodes
        = (vdrive->fsm_address - 1) * vdrive->block_size / AGG_INODE_SZ;

    vdrive->inodes = (inode*)calloc(vdrive->num_inodes, sizeof(inode));
    get_inode(vdrive->fd, &(vdrive->inodes[0]), 0, vdrive->block_size);

    vdrive->open_files = (size_t*)calloc(vdrive->num_inodes, sizeof(size_t));
    vdrive->num_open_files = 1;

    /* Get file count. */
    vdrive->num_files = vdrive->inodes[0].bytes / (MAX_FILE_NAME + sizeof(size_t));

    /* Set up available i-node book keeping. */
    vdrive->inodes_avail = (size_t*)calloc(vdrive->num_inodes, sizeof(size_t));
    for(i = 1; i < vdrive->num_inodes; ++i){
        vdrive->inodes_avail[i] = i;
    }
    ix = vdrive->num_inodes;
    for(i = 0; i < vdrive->inodes[0].bytes; i += MAX_FILE_NAME + sizeof(size_t)){
        vhd_read_file(vdrive, 0, &inode_number, sizeof(size_t), i);
        remove_in_order(inode_number, vdrive->inodes_avail, ix--);
    }
    return vdrive;
}

size_t vhd_open_file(vhd* vdrive, const char* file_name)
{
    char name[MAX_FILE_NAME];
    size_t nbytes, inode_number, ix;
    /* Search root directory for matching file name. */
    nbytes = vdrive->inodes[0].bytes;
    for(ix = 0; ix < nbytes; ix += MAX_FILE_NAME + sizeof(size_t)){
        vhd_read_file(vdrive, 0, name, MAX_FILE_NAME, ix + sizeof(size_t));
        if(!strcmp(file_name, (const char*)name)){
            vhd_read_file(vdrive, 0, &inode_number, sizeof(size_t), ix);
            get_inode(vdrive->fd, &(vdrive->inodes[inode_number]), inode_number,
                      vdrive->block_size);
            if(binary_search(inode_number, vdrive->open_files, 0,
                             vdrive->num_open_files - 1) == (size_t)-1){
                insert_in_order(inode_number, vdrive->open_files,
                                vdrive->num_open_files, vdrive->num_inodes);
                ++vdrive->num_open_files;
            }
            return inode_number;
        }
    }
    /* File does not exist. Create new file entry in root directory. */
    ix = 0;
    if(vdrive->num_files < vdrive->num_inodes){
        ix = vdrive->inodes_avail[0];
        remove_in_order(ix, vdrive->inodes_avail,
                        vdrive->num_inodes - vdrive->num_files);
        ++vdrive->num_files;
        vhd_write_file(vdrive, 0, &ix, sizeof(size_t), vdrive->inodes[0].bytes);
        vhd_write_file(vdrive, 0, (void*)file_name, MAX_FILE_NAME,
                       vdrive->inodes[0].bytes + sizeof(size_t));
        vdrive->inodes[0].bytes += MAX_FILE_NAME + sizeof(size_t);
        set_inode(vdrive->fd, &(vdrive->inodes[0]), 0, vdrive->block_size);
    }
    insert_in_order(ix, vdrive->open_files, vdrive->num_open_files,
                    vdrive->num_inodes);
    ++vdrive->num_open_files;
    return ix;
}

size_t vhd_file_size(vhd* vdrive, const char* file_name)
{
    char name[MAX_FILE_NAME];
    inode i_node;
    size_t nbytes, inode_number, ix;
    /* Search root directory for matching file name. */
    nbytes = vdrive->inodes[0].bytes;
    for(ix = 0; ix < nbytes; ix += MAX_FILE_NAME + sizeof(size_t)){
        vhd_read_file(vdrive, 0, name, MAX_FILE_NAME, ix + sizeof(size_t));
        if(!strcmp(file_name, (const char*)name)){
            vhd_read_file(vdrive, 0, &inode_number, sizeof(size_t), ix);
            get_inode(vdrive->fd, &i_node, inode_number, vdrive->block_size);
            return i_node.bytes;
        }
    }
    return 0;
}

size_t vhd_write_file(vhd* vdrive,
                      size_t fd,
                      void* buffer,
                      size_t nbytes,
                      size_t offset)
{
    inode i_node;
    size_t start_block, start_byte, bytes_written, max_bytes, rand_block;
    size_t ix, iy, iz, iw;
    size_t s_indirect[INODE_POINT_SZ], d_indirect[INODE_POINT_SZ],
        t_indirect[INODE_POINT_SZ];
    unsigned char* block;

    if(!vdrive)
        return 0;

    if(binary_search(fd, vdrive->open_files, 0, vdrive->num_open_files - 1)
       == (size_t)-1){
        return 0;
    }

    get_inode(vdrive->fd, &i_node, fd, vdrive->block_size);
    max_bytes = vdrive->block_size * MAX_FILE_BLOCKS;

    if(offset >= max_bytes)
        return 0;

    nbytes = (nbytes + offset < max_bytes)
        ? nbytes : max_bytes - ((nbytes + offset) - max_bytes);

    start_block = offset / vdrive->block_size;
    start_byte = offset % vdrive->block_size;

    block = (unsigned char*)calloc(vdrive->block_size, sizeof(unsigned char));

    bytes_written = 0;
    /* Block falls within direct pointers of i-node. */
    while(start_block < INODE_POINT_SZ){
        if(i_node.direct[start_block] == (size_t)-1){
            srand((unsigned int)(time(0)));
            do{
                rand_block = (rand() % (vdrive->num_blocks + 1));
            }while(!get_block_use(vdrive->fd, rand_block, vdrive->fsm_address,
                                  vdrive->block_size));
            i_node.direct[start_block] = rand_block;
            set_block_use(vdrive->fd, 0, rand_block, vdrive->fsm_address,
                          vdrive->block_size);
        }
        get_block(vdrive->fd, block, i_node.direct[start_block],
                  vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *(block + start_byte + ix)
                = *((unsigned char*)(buffer + bytes_written++));
            if(bytes_written == nbytes){
                set_block(vdrive->fd, block, i_node.direct[start_block],
                          vdrive->block_size);
                free(block);
                if(i_node.bytes - offset < bytes_written){
                    i_node.bytes += bytes_written - (i_node.bytes - offset);
                    set_inode(vdrive->fd, &i_node, fd, vdrive->block_size);
                }
                return bytes_written;
            }
        }
        set_block(vdrive->fd, block, i_node.direct[start_block],
                  vdrive->block_size);
        ++start_block;
        start_byte = 0;
    }
    /* Block falls within single indirect range. */
    get_indirect_block(vdrive->fd, s_indirect, i_node.s_indirect,
                       vdrive->block_size, vdrive->num_inodes);
    while(start_block >= INODE_POINT_SZ && start_block < INODE_POINT_SZ * 2){
        iy = start_block - INODE_POINT_SZ;
        if(s_indirect[iy] == (size_t)-1){
            srand((unsigned int)(time(0)));
            do{
                rand_block = (rand() % (vdrive->num_blocks + 1));
            }while(!get_block_use(vdrive->fd, rand_block, vdrive->fsm_address,
                                  vdrive->block_size));
            s_indirect[iy] = rand_block;
            set_block_use(vdrive->fd, 0, rand_block, vdrive->fsm_address,
                          vdrive->block_size);
            set_indirect_block(vdrive->fd, s_indirect, i_node.s_indirect,
                               vdrive->block_size,
                               vdrive->num_inodes);
        }

        get_block(vdrive->fd, block, s_indirect[iy], vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *(block + start_byte + ix)
                = *((unsigned char*)(buffer + bytes_written++));
            if(bytes_written == nbytes){
                set_block(vdrive->fd, block, s_indirect[iy],
                          vdrive->block_size);
                free(block);
                if(i_node.bytes - offset < bytes_written){
                    i_node.bytes += bytes_written - (i_node.bytes - offset);
                    set_inode(vdrive->fd, &i_node, fd, vdrive->block_size);
                }
                return bytes_written;
            }
        }
        set_block(vdrive->fd, block, s_indirect[iy],
                  vdrive->block_size);
        ++start_block ;
        start_byte = 0;
    }
    /* Block falls within double indirect range. */
    get_indirect_block(vdrive->fd, d_indirect, i_node.d_indirect,
                       vdrive->block_size, vdrive->num_inodes);
    while(start_block >= INODE_POINT_SZ * 2
          && start_block < INODE_POINT_SZ * 2 + (INODE_POINT_SZ * INODE_POINT_SZ)){
        iz = (start_block - (INODE_POINT_SZ * 2)) / INODE_POINT_SZ;
        iy = (start_block - (INODE_POINT_SZ * 2)) % INODE_POINT_SZ;
        get_indirect_block(vdrive->fd, s_indirect, d_indirect[iz],
                           vdrive->block_size, vdrive->num_inodes);
        if(s_indirect[iy] == (size_t)-1){
            srand((unsigned int)(time(0)));
            do{
                rand_block = (rand() % (vdrive->num_blocks + 1));
            }while(!get_block_use(vdrive->fd, rand_block,
                                  vdrive->fsm_address, vdrive->block_size));
            s_indirect[iy] = rand_block;
            set_block_use(vdrive->fd, 0, rand_block, vdrive->fsm_address,
                          vdrive->block_size);
            set_indirect_block(vdrive->fd, s_indirect, d_indirect[iz],
                               vdrive->block_size, vdrive->num_inodes);
        }
        get_block(vdrive->fd, block, s_indirect[iy], vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *(block + start_byte + ix) = *((unsigned char*)(buffer + bytes_written++));
            if(bytes_written == nbytes){
                set_block(vdrive->fd, block, s_indirect[iy],
                          vdrive->block_size);
                free(block);
                if(i_node.bytes - offset < bytes_written){
                    i_node.bytes +=
                        bytes_written - (i_node.bytes - offset);
                    set_inode(vdrive->fd, &i_node, fd, vdrive->block_size);
                }
                return bytes_written;
            }
        }
        set_block(vdrive->fd, block, s_indirect[iy], vdrive->block_size);
        ++start_block;
        start_byte = 0;
    }

    /* Block falls within triple indirect range. */
    get_indirect_block(vdrive->fd, t_indirect, i_node.t_indirect,
                       vdrive->block_size, vdrive->num_inodes);
    while(start_block >= INODE_POINT_SZ * 2 + (INODE_POINT_SZ * INODE_POINT_SZ)
          && start_block < MAX_FILE_BLOCKS){
        iw = start_block - ((INODE_POINT_SZ * 2) + (INODE_POINT_SZ * INODE_POINT_SZ)) / (INODE_POINT_SZ * INODE_POINT_SZ);
        iz = start_block - ((INODE_POINT_SZ * 2) + (INODE_POINT_SZ * INODE_POINT_SZ)) / INODE_POINT_SZ;
        iy = start_block - ((INODE_POINT_SZ * 2) + (INODE_POINT_SZ * INODE_POINT_SZ)) % INODE_POINT_SZ;

        get_indirect_block(vdrive->fd, d_indirect, t_indirect[iw],
                           vdrive->block_size, vdrive->num_inodes);

        get_indirect_block(vdrive->fd, s_indirect, d_indirect[iz],
                           vdrive->block_size, vdrive->num_inodes);

        if(s_indirect[iy] == (size_t)-1){
            srand((unsigned int)(time(0)));
            do{
                rand_block = (rand() % (vdrive->num_blocks + 1));
            }while(!get_block_use(vdrive->fd, rand_block,
                                  vdrive->fsm_address,
                                  vdrive->block_size));
            s_indirect[iy] = rand_block;
            set_block_use(vdrive->fd, 0, rand_block,
                          vdrive->fsm_address, vdrive->block_size);
            set_indirect_block(vdrive->fd, s_indirect, d_indirect[iz],
                               vdrive->block_size, vdrive->num_inodes);
        }
        get_block(vdrive->fd, block, s_indirect[iy],
                  vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *(block + start_byte + ix)
                = *((unsigned char*)(buffer + bytes_written++));
            if(bytes_written == nbytes){
                set_block(vdrive->fd, block, s_indirect[iy],
                          vdrive->block_size);
                free(block);
                if(i_node.bytes - offset < bytes_written){
                    i_node.bytes +=
                        bytes_written - (i_node.bytes - offset);
                    set_inode(vdrive->fd, &i_node, fd,
                              vdrive->block_size);
                }
                return bytes_written;
            }
        }
        set_block(vdrive->fd, block, s_indirect[iy],
                  vdrive->block_size);
        ++start_block;
        start_byte = 0;

    }
    free(block);
    if(i_node.bytes - offset < bytes_written){
        i_node.bytes +=
            bytes_written - (i_node.bytes - offset);
        set_inode(vdrive->fd, &i_node, fd,
                  vdrive->block_size);
    }
    return bytes_written;
}

size_t vhd_read_file(vhd* vdrive,
                     size_t fd,
                     void* buffer,
                     size_t nbytes,
                     size_t offset)
{
    inode i_node;
    size_t start_block, start_byte, bytes_read, ix, iy, iz, iw;
    size_t s_indirect[INODE_POINT_SZ], d_indirect[INODE_POINT_SZ],
        t_indirect[INODE_POINT_SZ];
    unsigned char* block;

    if(!vdrive)
        return 0;

    if(binary_search(fd, vdrive->open_files, 0, vdrive->num_open_files - 1)
       == (size_t)-1){
        return 0;
    }

    get_inode(vdrive->fd, &i_node, fd, vdrive->block_size);

    nbytes = (nbytes + offset < i_node.bytes)
        ? nbytes : nbytes - ((nbytes + offset) - i_node.bytes);

    start_block = offset / vdrive->block_size;
    start_byte = offset % vdrive->block_size;

    block = (unsigned char*)calloc(vdrive->block_size, sizeof(unsigned char));

    bytes_read = 0;

    /* Block falls within direct pointers of i-node. */
    while(start_block < INODE_POINT_SZ){
        get_block(vdrive->fd, block, i_node.direct[start_block],
                  vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *((unsigned char*)(buffer + bytes_read++)) = *(block + start_byte + ix);
            if(bytes_read == nbytes){
                free(block);
                return bytes_read;
            }
        }
        ++start_block;
        start_byte = 0;
    }
    /* Block falls within single indirect range. */
    get_indirect_block(vdrive->fd, s_indirect, i_node.s_indirect,
                       vdrive->block_size, vdrive->num_inodes);
    while(start_block >= INODE_POINT_SZ && start_block < INODE_POINT_SZ * 2){
        iy = start_block - INODE_POINT_SZ;
        get_block(vdrive->fd, block, s_indirect[iy], vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *((unsigned char*)(buffer + bytes_read++))
                = *(block + start_byte + ix);
            if(bytes_read == nbytes){
                free(block);
                return bytes_read;
            }
        }
        ++start_block;
        start_byte = 0;
    }
    /* Block falls within double indirect range. */
    get_indirect_block(vdrive->fd, d_indirect, i_node.d_indirect,
                       vdrive->block_size, vdrive->num_inodes);
    while(start_block >= INODE_POINT_SZ * 2
          && start_block < INODE_POINT_SZ * 2 + (INODE_POINT_SZ * INODE_POINT_SZ)){
        iz = (start_block - (INODE_POINT_SZ * 2)) / INODE_POINT_SZ;
        iy = (start_block - (INODE_POINT_SZ * 2)) % INODE_POINT_SZ;
        get_indirect_block(vdrive->fd, s_indirect, d_indirect[iz],
                           vdrive->block_size, vdrive->num_inodes);
        get_block(vdrive->fd, block, s_indirect[iy], vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *((unsigned char*)(buffer + bytes_read++))
                = *(block + start_byte + ix);
            if(bytes_read == nbytes){
                free(block);
                return bytes_read;
            }
        }
        ++start_block;
        start_byte = 0;
    }

    /* Block falls within triple indirect range. */
    get_indirect_block(vdrive->fd, t_indirect, i_node.t_indirect,
                       vdrive->block_size, vdrive->num_inodes);
    while(start_block >= INODE_POINT_SZ * 2 + (INODE_POINT_SZ * INODE_POINT_SZ)
          && start_block < MAX_FILE_BLOCKS){
        iw = start_block - ((INODE_POINT_SZ * 2) + (INODE_POINT_SZ * INODE_POINT_SZ)) / (INODE_POINT_SZ * INODE_POINT_SZ);
        iz = start_block - ((INODE_POINT_SZ * 2) + (INODE_POINT_SZ * INODE_POINT_SZ)) / INODE_POINT_SZ;
        iy = start_block - ((INODE_POINT_SZ * 2) + (INODE_POINT_SZ * INODE_POINT_SZ)) % INODE_POINT_SZ;

        get_indirect_block(vdrive->fd, d_indirect, t_indirect[iw],
                           vdrive->block_size, vdrive->num_inodes);

        get_indirect_block(vdrive->fd, s_indirect, d_indirect[iz],
                           vdrive->block_size, vdrive->num_inodes);

        get_block(vdrive->fd, block, s_indirect[iy],
                  vdrive->block_size);
        for(ix = 0; ix < vdrive->block_size - start_byte; ++ix){
            *((unsigned char*)(buffer + bytes_read++))
                = *(block + start_byte + ix);
            if(bytes_read == nbytes){
                free(block);
                return bytes_read;
            }
        }
        ++start_block;
        start_byte = 0;
    }
    free(block);
    return bytes_read;
}

void vhd_close_file(vhd* vdrive, size_t fd)
{
    if(!vdrive)
        return;

    if(binary_search(fd, vdrive->open_files, 0, vdrive->num_open_files)
       != (size_t)-1){
        remove_in_order(fd, vdrive->open_files, vdrive->num_open_files);
        --vdrive->num_open_files;
    }
}

void vhd_remove_file(vhd* vdrive, const char* file_name)
{
    char name[MAX_FILE_NAME];
    size_t nbytes, inode_number, inode_buffer, ix, iy, iz;
    size_t indirect_block[INODE_POINT_SZ], d_indirect_block[INODE_POINT_SZ],
        t_indirect_block[INODE_POINT_SZ];
    if(!vdrive)
        return;

    /* Search root directory for matching file name. */
    nbytes = vdrive->inodes[0].bytes;
    for(ix = 0; ix < nbytes; ix += MAX_FILE_NAME + sizeof(size_t)){
        vhd_read_file(vdrive, 0, name, MAX_FILE_NAME, ix + sizeof(size_t));
        /* Match found. */
        if(!strcmp(file_name, (const char*)name)){
            vhd_read_file(vdrive, 0, &inode_number, sizeof(size_t), ix);
            get_inode(vdrive->fd, &(vdrive->inodes[inode_number]), inode_number,
                      vdrive->block_size);
            /* Remove only if file is closed. */
            if(binary_search(inode_number, vdrive->open_files, 0,
                             vdrive->num_open_files - 1) == (size_t)-1){
                printf("%zu\n", ix);
                /* Remove entry in root directory. */
                for(; ix < nbytes; ix += MAX_FILE_NAME + sizeof(size_t)){
                    if(ix + MAX_FILE_NAME + sizeof(size_t) < nbytes){
                        vhd_read_file(vdrive, 0, &inode_buffer, sizeof(size_t),
                                      ix + MAX_FILE_NAME + sizeof(size_t));
                        vhd_read_file(vdrive, 0, name, MAX_FILE_NAME,
                                      ix + MAX_FILE_NAME + 2 * sizeof(size_t));
                        vhd_write_file(vdrive, 0, &inode_buffer, sizeof(size_t),
                                       ix);
                        vhd_write_file(vdrive, 0, name, MAX_FILE_NAME,
                                   ix + sizeof(size_t));
                    }
                    else{
                        inode_buffer = (size_t)-1;
                        for(iy = 0; iy < MAX_FILE_NAME; ++iy){
                            name[iy] = '\0';
                        }
                        vhd_write_file(vdrive, 0, &inode_buffer, sizeof(size_t),
                                       ix);
                        vhd_write_file(vdrive, 0, name, MAX_FILE_NAME,
                                   ix + sizeof(size_t));
                    }
                }
                vdrive->inodes[0].bytes -= MAX_FILE_NAME + sizeof(size_t);
                /* Fix direct pointer fields of inode. */
                for(ix = 0; ix < INODE_POINT_SZ; ++ix){
                    if(vdrive->inodes[inode_number].direct[ix] != (size_t)-1){
                        set_block_use(vdrive->fd, 1,
                                      vdrive->inodes[inode_number].direct[ix],
                                      vdrive->fsm_address,
                                      vdrive->block_size);
                        vdrive->inodes[inode_number].direct[ix] = (size_t)-1;
                    }
                }
                /* Single indirect block. */
                get_indirect_block(vdrive->fd, indirect_block,
                                   vdrive->inodes[inode_number].s_indirect,
                                   vdrive->block_size, vdrive->num_inodes);
                for(ix = 0; ix < INODE_POINT_SZ; ++ix){
                    if(indirect_block[ix] != (size_t)-1){
                        set_block_use(vdrive->fd, 1,
                                      indirect_block[ix],
                                      vdrive->fsm_address,
                                      vdrive->block_size);
                        indirect_block[ix] = (size_t)-1;
                    }
                }
                set_indirect_block(vdrive->fd, indirect_block,
                                   vdrive->inodes[inode_number].s_indirect,
                                   vdrive->block_size, vdrive->num_inodes);
                /* Double indirection block. */
                get_indirect_block(vdrive->fd, indirect_block,
                                   vdrive->inodes[inode_number].d_indirect,
                                   vdrive->block_size, vdrive->num_inodes);
                for(ix = 0; ix < INODE_POINT_SZ; ++ix){
                    get_indirect_block(vdrive->fd, d_indirect_block,
                                       indirect_block[ix],
                                       vdrive->block_size, vdrive->num_inodes);
                    for(iy = 0; iy < INODE_POINT_SZ; ++iy){
                        if(d_indirect_block[iy] != (size_t)-1){
                            set_block_use(vdrive->fd, 1,
                                          d_indirect_block[iy],
                                          vdrive->fsm_address,
                                          vdrive->block_size);
                            d_indirect_block[iy] = (size_t)-1;
                        }
                    }
                    set_indirect_block(vdrive->fd, d_indirect_block,
                                       indirect_block[ix],
                                       vdrive->block_size, vdrive->num_inodes);
                }
                set_indirect_block(vdrive->fd, indirect_block,
                                   vdrive->inodes[inode_number].d_indirect,
                                   vdrive->block_size, vdrive->num_inodes);
                /* Triple indirection block. */
                get_indirect_block(vdrive->fd, indirect_block,
                                   vdrive->inodes[inode_number].t_indirect,
                                   vdrive->block_size, vdrive->num_inodes);
                for(ix = 0; ix < INODE_POINT_SZ; ++ix){
                    get_indirect_block(vdrive->fd, d_indirect_block,
                                       indirect_block[ix],
                                       vdrive->block_size, vdrive->num_inodes);
                    for(iy = 0; iy < INODE_POINT_SZ; ++iy){
                        get_indirect_block(vdrive->fd, t_indirect_block,
                                           d_indirect_block[iy], vdrive->block_size,
                                           vdrive->num_inodes);
                        for(iz = 0; iz < INODE_POINT_SZ; ++iz){
                            if(t_indirect_block[iz] != (size_t)-1){
                                set_block_use(vdrive->fd, 1,
                                              t_indirect_block[iz],
                                              vdrive->fsm_address,
                                              vdrive->block_size);
                                t_indirect_block[iz] = (size_t)-1;
                            }
                        }
                        set_indirect_block(vdrive->fd, t_indirect_block,
                                           d_indirect_block[iy], vdrive->block_size,
                                           vdrive->num_inodes);
                    }
                    set_indirect_block(vdrive->fd, d_indirect_block,
                                       indirect_block[ix],
                                       vdrive->block_size, vdrive->num_inodes);
                }
                set_indirect_block(vdrive->fd, indirect_block,
                                   vdrive->inodes[inode_number].t_indirect,
                                   vdrive->block_size, vdrive->num_inodes);
                set_inode(vdrive->fd, &(vdrive->inodes[inode_number]), inode_number,
                          vdrive->block_size);
                insert_in_order(inode_number, vdrive->inodes_avail,
                                vdrive->num_inodes - vdrive->num_files,
                                vdrive->num_inodes);
                --vdrive->num_files;
            }
        }
    }
}

void vhd_unmount(vhd** vdrive)
{
    if(!*vdrive)
        return;
    close((*vdrive)->fd);
    if((*vdrive)->inodes){
        free((*vdrive)->inodes);
        (*vdrive)->inodes = 0;
    }
    if((*vdrive)->open_files){
        free((*vdrive)->open_files);
        (*vdrive)->open_files = 0;
    }
    if((*vdrive)->inodes_avail){
        free((*vdrive)->inodes_avail);
        (*vdrive)->inodes_avail = 0;
    }
    free(*vdrive);
    *vdrive = 0;
    return;
}

void vhd_state(const vhd* vdrive)
{
    if(!vdrive)
        return;

    printf("Disk Size:       %zu bytes.\n",
           vdrive->num_blocks * vdrive->block_size);
    printf("Block Size:      %zu bytes.\n", vdrive->block_size);
    printf("Blocks:          %zu blocks.\n", vdrive->num_blocks);
    printf("Max File Size:   %zu bytes.\n", vdrive->block_size * MAX_FILE_BLOCKS);
    printf("Inodes:          %zu.\n", vdrive->num_inodes);
    printf("Inode Start:     Block %zu.\n", vdrive->inode_address);
    printf("FSM Start:       Block %zu.\n", vdrive->fsm_address);
    printf("FS Start:        Block %zu.\n", vdrive->fs_address);
}

/*******************************************************************************
 * Static function definitions.
 ******************************************************************************/
static void insert_in_order(size_t element,
                            size_t* array,
                            size_t num_elements,
                            size_t capacity)
{
    size_t i, j;

    if(num_elements == capacity)
        return;

    for(i = 0; i < num_elements; ++i){
        if(element < array[i]){
            for(j = num_elements; j > i; --j){
                array[j] = array[j-1];
            }
            array[i] = element;
            return;
        }
    }
    array[num_elements] = element;
}

static size_t binary_search(size_t element, size_t* array, size_t low, size_t high)
{
    while(low <= high){
        size_t middle = low + (high - low) / 2;
        if(element < array[middle])
            high = middle - 1;
        else if(element > array[middle])
            low = middle + 1;
        else
            return middle;
    }
    return (size_t)-1;
}

static void remove_in_order(size_t element, size_t* array, size_t num_elements)
{
    size_t i;
    size_t rtn = binary_search(element, array, 0, num_elements);

    if(rtn != (size_t)-1){
        for(i = rtn; i < num_elements - 1; ++i){
            array[i] = array[i+1];
        }
    }
}

static unsigned char byte_pack(unsigned char old_byte,
                               unsigned char bit_offset,
                               unsigned char bit_count,
                               unsigned char value)
{
    char mask = (1 << bit_count) - 1;
    mask <<= bit_offset;
    old_byte &= ~mask;
    old_byte |= value << bit_offset;
    return old_byte;
}

static void get_block(int fd,
                      unsigned char* buffer,
                      size_t number,
                      size_t block_byte)
{
    void* vp;
    size_t i;
    for(i = 0; i < block_byte; ++i){
        vp = &buffer[i];
        pread(fd, vp, sizeof(unsigned char), i + block_byte * number);
    }
}

static void set_block(int fd,
                      unsigned char* buffer,
                      size_t number,
                      size_t block_byte)
{
    void* vp;
    size_t i;
    for(i = 0; i < block_byte; ++i){
        vp = &buffer[i];
        pwrite(fd, vp, sizeof(unsigned char), i + block_byte * number);
    }
}

static void set_block_use(int fd,
                          unsigned char value,
                          size_t block_number,
                          size_t fsm_address,
                          size_t block_byte)
{
    size_t offset = block_number / (block_byte * 8);
    size_t ix = (block_number - offset * block_byte) / 8;
    size_t bit = (block_number - offset * block_byte) % 8;
    unsigned char* block
        = (unsigned char*)calloc(block_byte, sizeof(unsigned char));

    get_block(fd, block, fsm_address + offset, block_byte);
    switch(value){
    case 0x00:
        block[ix] = byte_pack(block[ix], 7 - bit, 1, 0x00);
        break;
    case 0x01:
        block[ix] = byte_pack(block[ix], 7 - bit, 1, 0x00);
        break;
    default:
        break;
    }
    set_block(fd, block, fsm_address + offset, block_byte);
    free(block);
}

static int get_block_use(int fd,
                         size_t block_number,
                         size_t fsm_address,
                         size_t block_byte)
{
    unsigned char byte;
    size_t offset = block_number / (block_byte * 8);
    size_t ix = (block_number - offset * block_byte) / 8;
    size_t bit = (block_number - offset * block_byte) % 8;
    unsigned char* block
        = (unsigned char*)calloc(block_byte, sizeof(unsigned char));

    get_block(fd, block, fsm_address + offset, block_byte);

    if(ix < block_byte){
        byte = block[ix] & 0xff;
        switch(bit){
        case 0:
            byte = block[ix] & 0x80;
            break;
        case 1:
            byte = block[ix] & 0x40;
            break;
        case 2:
            byte = block[ix] & 0x20;
            break;
        case 3:
            byte = block[ix] & 0x10;
            break;
        case 4:
            byte = block[ix] & 0x08;
            break;
        case 5:
            byte = block[ix] & 0x04;
            break;
        case 6:
            byte = block[ix] & 0x02;
            break;
        case 7:
            byte = block[ix] & 0x01;
            break;
        }
        switch(byte){
        case 0x80:
        case 0x40:
        case 0x20:
        case 0x10:
        case 0x08:
        case 0x04:
        case 0x02:
        case 0x01:
            return 1;
            break;
        }
    }
    return 0;
}

static void get_inode(int fd, inode* i_node, size_t number, size_t block_byte)
{
    void* vp;
    size_t ix;

    vp = &(i_node->bytes);
    pread(fd, vp, sizeof(size_t), block_byte + INODE_SZ * number);

    for(ix = 0; ix < INODE_POINT_SZ; ++ix){
        vp = &(i_node->direct[ix]);
        pread(fd, vp, sizeof(size_t),
              block_byte + sizeof(size_t)
              + INODE_SZ * number
              + ix * sizeof(size_t));
    }

    vp = &(i_node->s_indirect);
    pread(fd, vp, sizeof(size_t),
          block_byte + sizeof(size_t)
          + INODE_SZ * number
          + ix++ * sizeof(size_t));

    vp = &(i_node->d_indirect);
    pread(fd, vp, sizeof(size_t),
          block_byte + sizeof(size_t)
          + INODE_SZ * number
          + ix++ * sizeof(size_t));

    vp = &(i_node->t_indirect);
    pread(fd, vp, sizeof(size_t),
          block_byte + sizeof(size_t)
          + INODE_SZ * number
          + ix++ * sizeof(size_t));
}

static void set_inode(int fd, inode* i_node, size_t number, size_t block_byte)
{
    void* vp;
    size_t ix;

    vp = &(i_node->bytes);
    pwrite(fd, vp, sizeof(size_t), block_byte + INODE_SZ * number);

    for(ix = 0; ix < INODE_POINT_SZ; ++ix){
        vp = &(i_node->direct[ix]);
        pwrite(fd, vp, sizeof(size_t),
               block_byte + sizeof(size_t)
               + INODE_SZ * number
               + ix * sizeof(size_t));
    }

    vp = &(i_node->s_indirect);
    pwrite(fd, vp, sizeof(size_t),
           block_byte + sizeof(size_t)
           + INODE_SZ * number
           + ix++ * sizeof(size_t));

    vp = &(i_node->d_indirect);
    pwrite(fd, vp, sizeof(size_t),
           block_byte + sizeof(size_t)
           + INODE_SZ * number
           + ix++ * sizeof(size_t));

    vp = &(i_node->t_indirect);
    pwrite(fd, vp, sizeof(size_t),
           block_byte + sizeof(size_t)
           + INODE_SZ * number
           + ix++ * sizeof(size_t));
}

static void get_indirect_block(int fd,
                               size_t* iblock,
                               size_t number,
                               size_t block_byte,
                               size_t num_inodes)
{
    void* vp = iblock;
    pread(fd, vp, INDIRECT_BLOCK_SZ,
          block_byte + (num_inodes * INODE_SZ) + (number * INDIRECT_BLOCK_SZ));
}

static void set_indirect_block(int fd,
                               size_t* iblock,
                               size_t number,
                               size_t block_byte,
                               size_t num_inodes)
{
    void* vp = iblock;
    pwrite(fd, vp, INDIRECT_BLOCK_SZ,
          block_byte + (num_inodes * INODE_SZ) + (number * INDIRECT_BLOCK_SZ));
}

static void sanitize(size_t* disk_byte,
                     size_t* block_byte,
                     size_t* num_blocks,
                     size_t* bit_map_blocks,
                     size_t* inode_blocks)
{
    size_t new_disk_byte, new_block_byte, bm_blocks, drive_blocks, total;
    double inode_block_ratio, inode_file_block_count;

    /* Sanitize input if outside bounds. */
    *disk_byte = *disk_byte < MIN_DRIVE_SZ ? MIN_DRIVE_SZ : *disk_byte;
    *disk_byte = *disk_byte > MAX_DRIVE_SZ ? MAX_DRIVE_SZ : *disk_byte;
    *block_byte = *block_byte < MIN_BLOCK_SZ ? MIN_BLOCK_SZ : *block_byte;
    *block_byte = *block_byte > MAX_BLOCK_SZ ? MAX_BLOCK_SZ : *block_byte;

    /* Adjust input if necessary. */
    if(*disk_byte % *block_byte != 0){
        for(new_disk_byte = *disk_byte;
            new_disk_byte >= MIN_DRIVE_SZ;
            new_disk_byte--){
            for(new_block_byte = *block_byte;
                new_block_byte >= MIN_BLOCK_SZ;
                --new_block_byte){
                if(new_disk_byte % new_block_byte == 0){
                    *disk_byte = new_disk_byte;
                    *block_byte = new_block_byte;
                    /* Find number of blocks fsm bit map takes up. */
                    bm_blocks = *num_blocks = *disk_byte / *block_byte;
                    for(; bm_blocks < (size_t)-1; bm_blocks++){
                        if(bm_blocks % (*block_byte) == 0){
                            if((bm_blocks / (*block_byte)) % 8 == 0){
                                *bit_map_blocks = ((bm_blocks / (*block_byte)) / 8);
                            }
                            else{
                                *bit_map_blocks =
                                    ((bm_blocks / (*block_byte)) / 8) + 1;
                            }
                            break;
                        }
                    }
                    /* Find number of blocks inodes take up. */
                    drive_blocks = (*disk_byte / *block_byte) - *bit_map_blocks - 1;
                    inode_block_ratio = AGG_INODE_SZ / *block_byte;
                    total = *inode_blocks = 0;
                    inode_file_block_count = 0.0;
                    while(total < drive_blocks){
                        inode_file_block_count
                            += inode_block_ratio + MAX_FILE_BLOCKS;
                        total = inode_file_block_count;
                        ++(*inode_blocks);
                    }
                    *inode_blocks = *inode_blocks * inode_block_ratio;
                    return;
                }
            }
        }
    }
    /* Find number of blocks fsm bit map takes up. */
    bm_blocks = *num_blocks = *disk_byte / *block_byte;
    for(; bm_blocks < (size_t)-1; bm_blocks++){
        if(bm_blocks % (*block_byte) == 0){
            if((bm_blocks / (*block_byte)) % 8 == 0){
                *bit_map_blocks = ((bm_blocks / (*block_byte)) / 8);
            }
            else{
                *bit_map_blocks = ((bm_blocks / (*block_byte)) / 8) + 1;
            }
            break;
        }
    }
    /* Find number of blocks inodes take up. */
    drive_blocks = (*disk_byte / *block_byte) - *bit_map_blocks - 1;
    inode_block_ratio = AGG_INODE_SZ / (double)(*block_byte);
    total = *inode_blocks = 0;
    inode_file_block_count = 0.0;
    while(total < drive_blocks){
        inode_file_block_count += inode_block_ratio + MAX_FILE_BLOCKS;
        total = inode_file_block_count;
        ++(*inode_blocks);
    }
    *inode_blocks = *inode_blocks * inode_block_ratio;
}
