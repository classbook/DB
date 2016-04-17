/**
 * @file core_memory.c
 * @author Kishore
 * @date March 26, 2016, 9:56 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/init.h"
#include "headers/core_memory.h"



static memory_t *units, *unit_buffer = NULL;
static memory_instance_t *instances;

static memory_t *unit_start;
static int instance_start=0;

static memory_t* instance_addUnit(memory_instance_t *instance);
static memory_t* unit_getNew(void);
static void unit_freeAll(memory_instance_t *instance);


/**
 * Initializes memory  
 */
void memory_init()
{
	ansi_sinfo("Initializing Memory Module...");
	
	int i=0;
	units = (memory_t *)malloc(sizeof(memory_t)*MEMORY_N_UNITS);
	unit_start = units;
	for (i=0; i<MEMORY_N_UNITS; i++)
	{
		units[i].filled = 0;
		units[i].sindex = 0;
		units[i].next = units+i+1;
	}
	units[MEMORY_N_UNITS-1].next = NULL;
	
	instances = (memory_instance_t *)malloc(sizeof(memory_instance_t)*MEMORY_N_INSTANCES);
	for (i=0; i<MEMORY_N_INSTANCES; i++)
	{
		instances[i].t_len = 0;
		instances[i].start = NULL;
		instances[i].end = NULL;
		instances[i].free = TRUE;
		instances[i].next = i+1;
	}
	instances[MEMORY_N_INSTANCES-1].next = -1;
	ansi_info("\rOK >> Memory Module.\n");
}

/**
 * Creates an memory instance in memory module.
 * @returns created memory instance identifier.
 */
int memory_create()
{
	if (instance_start==-1){
		ansi_error("Maximum number of memory instances reached...");
		return -1;
	}
	assert(instances[instance_start].free);
	
	int freeinstance = instance_start;
	instances[freeinstance].free = FALSE;
	instances[freeinstance].t_len = 0;

	memory_t *freeunit = unit_getNew();
	if (freeunit==NULL){
		ansi_error("No free units found. Couldn't create memory instance.");
		return -1;
	}
	instances[freeinstance].start = freeunit;
	instances[freeinstance].end = freeunit;
	
	instance_start = instances[instance_start].next;
	
	return freeinstance;
}

/**
 * Deletes memory instance with id `instance_index`
 * @param instance_index : Id of instance that needs to be deleted.
 */
void memory_delete(int instance_index)
{
	assert(instance_index>=0 && instance_index<MEMORY_N_INSTANCES);
	
	assert(!instances[instance_index].free);

	unit_freeAll(instances + instance_index);
	instances[instance_index].next = instance_start;
	instances[instance_index].free = TRUE;
	instance_start = instance_index;
}


/**
 * Appends data of length len to memory instance.
 * @param mem_index: Memory instance id.
 * @param data: Character array that needs to be appended.
 * @param len: Length of string that needs to be appended.
 * @returns Amount of data appended.
 */
int memory_appendmem(int mem_index, char *data, int len)
{
	assert(mem_index>=0 && mem_index<MEMORY_N_INSTANCES);
	assert(!instances[mem_index].free);
	
	memory_t *unit = instances[mem_index].end;

	int i=0;
	for (i=0; i<len; i++)
	{
		if (unit->filled==MEMORY_UNIT_MAXSIZE){
			unit = instance_addUnit(instances + mem_index);
			if (unit==NULL)
				return i;
		}
		unit->data[unit->filled++] = data[i];
		instances[mem_index].t_len++;
	}
	return len;
}

/**
 * Append string to memory instance.
 * @param mem_index: Memory instance identifier.
 * @param data: String that needs to be appended.
 * @returns amount of length appended.
 */
int memory_appendstr(int mem_index, char *data)
{
	assert(mem_index>=0 && mem_index<MEMORY_N_INSTANCES);
	assert(!instances[mem_index].free);
	
	memory_t *unit = instances[mem_index].end;
	int i = 0;
	
	for (i=0; (i==0)?1:data[i-1]!='\0'; i++)
	{
		if (unit->filled==MEMORY_UNIT_MAXSIZE){
			unit = instance_addUnit(instances + mem_index);
			if (unit==NULL)
				return i;
		}
		unit->data[unit->filled++] = data[i];
		instances[mem_index].t_len++;
	}
	return 1;
}

/**
 * Initializes getting memory from memory instance in a procedural way.
 * @param mem_index: Memory instance identifier.
 * @returns reference to memory instance which could be used for further
 * retrieval.
 */
memory_get_t memory_init_get(int mem_index)
{
	assert(mem_index>=0 && mem_index<MEMORY_N_INSTANCES);
	assert(!instances[mem_index].free);
	
	memory_get_t mg;
	mg._memory = instances[mem_index].start;
	mg.index = 0;
	mg.len = mg._memory->filled;
	mg.data = mg._memory->data;
	
	return mg;
}

memory_pointer_t memory_pointer_start(int mem_index)
{
	memory_get_t mg = memory_init_get(mem_index);
	memory_pointer_t pointer;
	pointer._memory = mg._memory;
	pointer.data = mg._memory->data;
	pointer.index = mg._memory->sindex;
	pointer.len = &mg._memory->filled;
	return pointer;
}

memory_pointer_t memory_pointer_next(memory_pointer_t pointer)
{
	assert(pointer._memory!=NULL);
	if (pointer._memory->next==NULL)
	{
		pointer._memory = NULL;
		pointer.data = NULL;
		pointer.index = -1;
		pointer.len = NULL;
		return pointer;
	}
	pointer._memory = pointer._memory->next;
	pointer.data = pointer._memory->data;
	pointer.index = pointer._memory->sindex;
	pointer.len = &pointer._memory->filled;
	return pointer;
}

memory_pointer_t memory_pointer_end(int mem_index)
{
	assert(mem_index>=0 && mem_index<MEMORY_N_INSTANCES);
	assert(!instances[mem_index].free);

	memory_t *mem = instances[mem_index].end;
	
	memory_pointer_t pointer;
	pointer._memory = mem;
	pointer.data = mem->data;
	pointer.index = mem->sindex;
	pointer.len = &mem->filled;
	return pointer;
}

/**
 * Get more data from memory instance referenced by `mg`.
 * @param mg: Reference to memory instance.
 */
void memory_get(memory_get_t *mg)
{
	assert(mg!=NULL);
	assert(mg->_memory!=NULL);

	mg->_memory = mg->_memory->next;
	if (mg->_memory==NULL){
		mg->data = NULL;
		mg->index=-1;
		mg->len=0;
		return;
	}
	mg->data = mg->_memory->data;
	mg->index = mg->_memory->sindex;
	mg->len = mg->_memory->filled;
}

memory_get_t memory_get_end(int mem_index)
{
	assert(mem_index>=0 && mem_index<MEMORY_N_INSTANCES);
	assert(!instances[mem_index].free);

	memory_get_t mg;
	mg._memory = instances[mem_index].start;
	mg.index = mg._memory->filled;
	mg.len = mg._memory->filled;
	mg.data = mg._memory->data;
	
	return mg;

}

/**
 * Get memory reference to memory instance at index `index`.
 * @param memory_index: Memory instance identifier.
 * @param index: Index of memory instance where the reference is needed.
 * @return memory instance reference.
 */
memory_get_t memory_get_index(int memory_index, int index)
{
	memory_get_t mg;
	mg = memory_init_get(memory_index);
	int i=0;
	while (1)
	{
		if (mg._memory==NULL)
		{
			return mg;
		}
		if ((i+mg.len)>index)
		{
			mg.index = index%MEMORY_UNIT_MAXSIZE;
			return mg;
		}
		i+=mg.len;
		memory_get(&mg);
	}
	return mg;
}

/**
 * Copies data in memory instance to memory buffer `mem`.
 * @param mem_index: Memory instance identifier.
 * @param index: Index from which the copy should occur.
 * @param len: Length of data to be copied.
 * @param mem: Memory buffer to which data needs to be copied.
 * @returns Amount of data copied.
 */
int memory_copy(int mem_index, int index, int len, char *mem)
{
	int i=0;
	memory_get_t mg = memory_get_index(mem_index, index);
	for(i=0; i<len; i++)
	{
		if (mg.index==mg.len){
			memory_get(&mg);
			if (mg._memory==NULL){
				mem[i]='\0';
				return i+1;
			}
		}
		mem[i] = *(mg.data+mg.index);
		mg.index++;
	}
	mem[i]='\0';
	return i;
}

/**
 * Prints memory stored in instance.
 * @param mem_index: Memory instance identifier.
 */
void memory_printAll(int mem_index)
{
	memory_print(mem_index, 0, -1);
}


/**
 * Prints memory stored in instance of length len starting from `index`.
 * @param mem_index: Memory instance identifier.
 * @param index: Index from which printing should start.
 * @param len: Length of data that should be printed. -1 if upto end should
 * be printed.
 */
void memory_print(int mem_index, int index, int len)
{
	memory_get_t mg = memory_get_index(mem_index, index);
	if (mg._memory==NULL)
		ansi_error("NO CONTENT.\n");
	int i;
	for (i=0; (i<len) || (len==-1); i++)
	{
		if (mg.index==mg.len)
		{
			memory_get(&mg);
			if (mg._memory==NULL)
				break;
		}
		printf("%c", mg.data[mg.index++]);
	}
}


static memory_t* instance_addUnit(memory_instance_t *instance)
{
	memory_t *unit = instance->end;

	assert(unit->next==NULL);
	
	unit->next = unit_getNew();
	return instance->end = unit->next;
}

static memory_t* unit_getNew(void)
{
	if (unit_start==NULL){
		ansi_error("Maximum number of memory units reached...");
		return NULL;
	}
	
	memory_t *freeunit = unit_start;
	freeunit->filled = 0;
	freeunit->sindex = 0;
	
	unit_start = freeunit->next;
	
	freeunit->next = NULL;
	return freeunit;
}

static void unit_freeAll(memory_instance_t *instance)
{
	assert(instance->end->next==NULL);
	
	instance->end->next = unit_start;
	unit_start = instance->start;
}