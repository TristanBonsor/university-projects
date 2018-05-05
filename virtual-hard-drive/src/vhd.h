/*******************************************************************************
 * File:    vhd.h
 *
 * Copyright (C) 2012, 2013  Tristan W. Bonsor
 *
 * This work is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 2.
 *
 * Description: API for virtual hard drive.
 ******************************************************************************/
#ifndef _VHD_H_
#define _VHD_H_

typedef struct vhd vhd;

size_t vhd_create(const char* drive_name, size_t disk_byte, size_t block_byte);

vhd* vhd_mount(const char* drive_name);

size_t vhd_open_file(vhd* vdrive, const char* file_name);

size_t vhd_file_size(vhd* vdrive, const char* file_name);

size_t vhd_write_file(vhd* vdrive,
                      size_t fd,
                      void* buffer,
                      size_t nbytes,
                      size_t offset);

size_t vhd_read_file(vhd* vdrive,
                     size_t fd,
                     void* buffer,
                     size_t nbytes,
                     size_t offset);

void vhd_close_file(vhd* vdrive, size_t fd);

void vhd_remove_file(vhd* vdrive, const char* file_name);

void vhd_unmount(vhd** vdrive);

void vhd_state(const vhd* vdrive);

#endif /* _VHD_H_ */
