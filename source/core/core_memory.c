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

void memory_delete(int instance_index)
{
	assert(instance_index>=0 && instance_index<MEMORY_N_INSTANCES);
	
	assert(!instances[instance_index].free);

	unit_freeAll(instances + instance_index);
	instances[instance_index].next = instance_start;
	instances[instance_index].free = TRUE;
	instance_start = instance_index;
}



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

void memory_get(memory_get_t *mg)
{
	assert(mg!=NULL);
	assert(mg->_memory!=NULL);

	mg->_memory = mg->_memory->next;
	if (mg->_memory==NULL)
		return;
	mg->data = mg->_memory->data;
	mg->index = mg->_memory->sindex;
	mg->len = mg->_memory->filled;
}

void memory_print(int mem_index)
{
	memory_t *unit = instances[mem_index].start;
	memory_get_t mg = memory_init_get(mem_index);
	for (; mg._memory!=NULL; memory_get(&mg))
	{
		int i;
		for (i=mg.index;i<mg.len; i++){
			printf("%c", mg.data[i]);
		}
	}
	printf("\n");
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