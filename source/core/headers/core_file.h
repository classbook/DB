/**
 * @file core_fileio.h
 * @author Kishore
 * @date March 25, 2016, 11:14 PM 
 */

#include "common/headers/init.h"

void* file_map(char *file);
int file_create(char *file, int prealloc_memory);
int file_size(char *file);
int file_exist(char *file);
int file_open(char *file);
void file_error();