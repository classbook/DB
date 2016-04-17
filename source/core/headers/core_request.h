/**
 * @file core_reqcontroller.h
 * @author Kishore
 * @date March 25, 2016, 10:55 PM 
 */

#include "common/headers/init.h"

void request_init(void);

int request_store();
inline int request_wait(int req_fd, request_get_t *req, int req_len, int wtime);

request_get_t request_add(int reqfd, int fd, char *interest, int create_memory, int data);
int request_modify(int req_fd, int request_id, char* interest, int data);
void request_delete(int req_fd, int request_index);