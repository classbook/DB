/**
 * @file core_socket.h
 * @author Kishore
 * @date March 19, 2016, 11:26 PM 
 */

#include "common/headers/init.h"

int socket_create_server(char *port);
inline int socket_listen(int server_fd);
void socket_make_nonblocking(int socket);
int socket_connect_to_server(char *ip, char *port);

int socket_accept(int server_fd);
int socket_accept_nonblocking(int server_fd);