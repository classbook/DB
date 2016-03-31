/**
 * @file core_memory.h
 * @author Kishore
 * @date March 26, 2016, 9:56 PM 
 */

#include "init.h"

void memory_init();
int memory_create();
void memory_delete(int instance_index);
int memory_appendmem(int mem_index, char *data, int len);
int memory_appendstr(int mem_index, char *data);
memory_get_t memory_init_get(int mem_index);
void memory_get(memory_get_t *mg);
void memory_print(int mem_index);