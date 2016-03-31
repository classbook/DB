/**
 * @file core_request.c
 * @author Kishore
 * @date March 25, 2016, 8:58 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/core_request.h"
#include "headers/core_epoll.h"
#include "headers/core_memory.h"

request_t *list=NULL;
int units = 0;

void request_init(void)
{
	ansi_sinfo("Initializing Request Controller Module...");
	list = malloc(sizeof(request_t)*REQUEST_MAXREQUESTS);
	int i=0;
	for (i=0; i<REQUEST_MAXREQUESTS; i++)
	{
		list[i]._next = i+1;
		list[i].isempty = TRUE;
	}
	list[REQUEST_MAXREQUESTS-1]._next = -1;
	ansi_info("\rOK >> Request Controller.\n");
}

int request_store()
{
	int epfd = epoll_create1(0);
	assert(epfd!=-1);
	return epfd;
}


/**
 * Wait for events to occur on fds stored in reqfd and store event
 * related data in req array buffer.
 * @param reqfd : requests store id.
 * @param req : request_get_t structure where the events list should be stored.
 * @param wtime : wait till events occur, or timeout...
 * @returns number of descriptors that have events occured.
 * 
 * Please note that the req buffer array should be of length 10.
 */
inline int request_wait(int reqfd, request_get_t *req, int wtime)
{
	struct epoll_event events[10];
	int status = epoll_wait(reqfd, events, 10, wtime);

	if (status==-1)
		return -1;
	
	int i=0;
	for (i=0; i<status; i++)
	{
		request_t *r = list + events[i].data.fd;
		req[i].req_id = events[i].data.fd;
		req[i].data = r->data;
		req[i].mem_index = r->mem_index;
		req[i].fd = r->fd;
		req[i].events = events[i].events;
	}
	return status;
}


int request_add(int reqfd, int fd, char *interest, int create_memory, int data)
{
	if (units==-1){
		ansi_error("No of requests overflown.\n");
		return 0;
	}
	assert(list[units].isempty);;
	epoll_add(reqfd, fd, interest, units);
	list[units].fd = fd;
	
	if (create_memory)
	{
		list[units].mem_index = memory_create();
	}
	else
	{
		list[units].mem_index = -1;
	}
	
	list[units].data = data;
	list[units].isempty = FALSE;
	units = list[units]._next;
	
	return 1;
}

/**
 * 
 * @param reqfd
 * @param request_id
 * @param interest
 * @return 
 */
int request_modify(int reqfd, int request_id, char* interest)
{
	return epoll_modify(reqfd, list[request_id].fd, request_id, interest);
}

void request_delete(int reqfd, int request_index)
{
	assert(!list[request_index].isempty);
	list[request_index].isempty = TRUE;
	list[request_index]._next = units;
	if (list[request_index].mem_index!=-1)
	{
		memory_delete(list[request_index].mem_index);
	}
	epoll_remove(reqfd, list[request_index].fd);
	units = request_index;
}