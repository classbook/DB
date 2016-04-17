/**
 * @file lib_http.c
 * @author Kishore
 * @date April 14, 2016, 10:33 AM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/lib_http.h"
#include "core/headers/core_memory.h"
#include "core/headers/core_request.h"


void http_request_init(http_request_t *request, int mem_index)
{
	request->is_parsed = FALSE;
	request->memory_index = mem_index;
	request->write_end = memory_pointer_start(mem_index);
	
	request->headers._end = memory_pointer_start(mem_index);
	request->headers._end_l = 0;
	request->headers.content_length = 0;
	request->headers.length = 0;
}


int http_request_read(http_request_t *request)
{
	request->memory_index;
	assert(request->is_parsed==FALSE);
	
	char *regex = "\r\n\r\n";
	int i=0;
	while (1)
	{
		printf("%d\n", i++);
		memory_pointer_t end = request->headers._end;
		if (end.index==*(end.len))
		{
			end = memory_pointer_next(end);
			if (end._memory==NULL){
				return 0;
				break;
			}
			request->headers._end = end;
		}
		
		int i;
		for (i=end.index; i<*(end.len); i++)
		{
			if (end.data[i]==regex[request->headers._end_l]){
				request->headers._end_l++;
				if (request->headers._end_l==4){
					request->is_parsed = 1;
					request->headers.length += i-end.index;
					return 1;
				}
				continue;
			}
			request->headers._end_l = 0;
		}
		request->headers.length += *(request->headers._end.len)-request->headers._end.index;
	}
}

int http_write(int fd, http_request_t *request)
{
	assert(request->is_parsed==TRUE);
	
	while (1)
	{
		assert(request->write_end._memory!=NULL);
		int buffer_len = *(request->write_end.len)-request->write_end.index;
		
		if (buffer_len==0){
			request->write_end = memory_pointer_next(request->write_end);
			if (request->write_end._memory==NULL)
			{
				// Done writing all the data :-)
				return 1;
			}
		}
		int w = write(fd, request->write_end.data, buffer_len);
		
		if (w==0){
			ansi_error("End of file while writing : %d\n", fd);
			return 1;
		}
		else if (w<=-1){
			assert(w>=-1);
			return 0;
		}
		else{
			request->write_end.index += w;
			return 0;
		}
	}
}



int http_parse()
{
	char *http_status = "";
}