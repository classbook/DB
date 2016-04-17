/**
 * @file core_memory.h
 * @author Kishore
 * @date March 26, 2016, 9:56 PM 
 */

#include "common/headers/init.h"

void memory_init();

int memory_create();
void memory_delete(int instance_index);

int memory_appendmem(int mem_index, char *data, int len);
int memory_appendstr(int mem_index, char *data);

int memory_copy(int mem_index, int index, int len, char *mem);

memory_get_t memory_init_get(int mem_index);
void memory_get(memory_get_t *mg);
memory_get_t memory_get_index(int memory_index, int index);
memory_get_t memory_get_end(int mem_index);

memory_pointer_t memory_pointer_start(int mem_index);
memory_pointer_t memory_pointer_next(memory_pointer_t pointer);
memory_pointer_t memory_pointer_end(int mem_index);

void memory_printAll(int mem_index);
void memory_print(int mem_index, int index, int len);