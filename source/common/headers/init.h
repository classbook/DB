/**
 * @file init.h
 * @author Kishore
 * @date April 16, 2016, 11:31 AM 
 */

#include "stdheaders.h"
#include "core/headers/init.h"
#include "libraries/headers/init.h"

#ifndef UNIQUE_COMMON_DEFINITIONS_IDENTIFIER
	#define UNIQUE_COMMON_DEFINITIONS_IDENTIFIER

	typedef struct{
		int fd;
		int mem_index;	// Implementation dependent
		int data;
		http_request_t http;
		int isempty;
		int _next;
	}request_t;
	
	typedef struct{
		int req_id;
		int fd;
		int data;
		int mem_index;
		http_request_t *http;
		uint32_t events;
	}request_get_t;

#endif