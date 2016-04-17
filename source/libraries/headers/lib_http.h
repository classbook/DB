/**
 * @file lib_http.h
 * @author Kishore
 * @date April 14, 2016, 10:33 AM 
 */

#include "common/headers/init.h"

void http_request_init(http_request_t *request, int mem_index);

int http_request_read(http_request_t *request);
int http_write(int fd, http_request_t *request);