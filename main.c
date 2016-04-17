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
#include "core/headers/core_regex.h"
#include "libraries/headers/lib_http.h"


void read_request(request_get_t request);
static int req_fd;
int main()
{
	memory_init();
	request_init();
	
	char *regex = "(*!:):* (*!\r)\r\n";
	char str[100] = "Content-Length : 5619\r\n";
	regex_cap_t caps[10];
	regex_status_t status = regex_parse_string(regex, str, caps, 10);
	regex_printStatusDetails(status);
	printf("(%.*s) == (%.*s)\n", caps[0].len, str+caps[0].index, caps[1].len, str+caps[1].index);
	return 0;
	req_fd = request_store();
	int server = socket_create_server("8080");
	socket_make_nonblocking(server);
	socket_listen(server);
	
	request_add(req_fd, server, "r", FALSE, -1);
	
	while (1)
	{
		request_get_t req[10];
		printf("WAITING\n");
		int n_requests = request_wait(req_fd, req, 10, -1);
		
		int i=0;
		for (i=0; i<n_requests; i++)
		{
			request_get_t request = req[i];
			
			if (request.fd==server)
			{
				if (epoll_can_read(request.events))
				{
					int client;
					while ((client = socket_accept_nonblocking(server))!=-1)
					{
						request_get_t req = request_add(req_fd, client, "r", TRUE, -1);
						http_request_init(req.http, req.mem_index);
					}
					printf("DONE\n");
					continue;
				}else{
					ansi_error("Not read server\n");
				}
			}
			else
			{
				if (epoll_can_read(request.events))
				{
					printf("READING\n");
					read_request(request);
					if (http_request_read(request.http)){
						request_modify(req_fd, request.req_id, "w", 0);
					}
				}
				else if (epoll_can_write(request.events))
				{
					if (http_write(request.fd, request.http)){
						request_delete(req_fd, request.req_id);
						close(request.fd);
					}
					else{
						request_modify(req_fd, request.req_id, "w", 0);
					}
				}
				else
				{
					ansi_error("Unknown error occurred. Closing file descriptor.\n");
					request_delete(req_fd, request.req_id);
					close(request.fd);
					continue;
				}
			}
		}
		printf("WAITed\n");
	}
	return 0;
}

void read_request(request_get_t request)
{
	int fd = request.fd;
	char buffer[512];
	while (1)
	{
		int r = read(fd, buffer, 512);
		assert(r>=-1);
		if (r==0){
			request_delete(req_fd, request.req_id);
			close(fd);
			return;
		}
		else if (r==-1)
		{
			break;
		}
		else
		{
			memory_appendmem(request.mem_index, buffer, r);
		}
	}
	
}