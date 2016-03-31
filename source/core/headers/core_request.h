/**
 * @file core_reqcontroller.h
 * @author Kishore
 * @date March 25, 2016, 10:55 PM 
 */

#include "init.h"

void request_init(void);

int request_store();
inline int request_wait(int reqfd, request_get_t *req, int wtime);

int request_add(int reqfd, int fd, char *interest, int create_memory, int data);
int request_modify(int reqfd, int request_id, char* interest);
void request_delete(int reqfd, int request_index);