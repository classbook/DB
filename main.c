/**
 * @file main.c
 * @author Kishore
 * @date March 19, 2016, 12:36 PM
 * 
 * @brief 
 * 
 * 
 */

#include "core/headers/core_ansi.h"
#include "core/headers/core_request.h"
#include "core/headers/core_memory.h"
#include "core/headers/core_socket.h"
#include "core/headers/core_epoll.h"

int main()
{
	memory_init();
	request_init();
	
	ansi_fg_bg_text(C_CYAN, C_DEFAULT, "\nDone Initializing.\n");
	int server = socket_create_server("8080");
	socket_make_nonblocking(server);
	
	int reqfd = request_store();
	
	
	while (1)
	{
		request_get_t req[10];
		int status = request_wait(reqfd, req, -1);
		if (status==-1)
			continue;
		
		int i=0;
		for (i=0; i<status; i++)
		{
			request_modify(reqfd, req[i].req_id, "");
		}
	}
	
	
	return 0;
}