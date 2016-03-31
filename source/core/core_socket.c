/**
 * @file lib_socket.c
 * @author Kishore
 * @date June 24, 2015, 8:51 PM
 * 
 * @brief Wrapper Library of sockets that provides API easier
 * to use and hides all the internal details of the implementation.
 */

#include "headers/core_socket.h"


/**
 * Creates and binds sockets to given port address.
 * @param Port : Port address to what socket should bind.
 * @return Socket address on success or -1 on error.
 * 
 * create_bind_socket is a wrapper function to create
 * and bind socket to a given port address. It tries to
 * get all available addresses of this system, and try
 * to bind the port to that address. When success returns
 * the socket address or -1 on error.
 */
int socket_create_server(char *port)
{
	// Get Address of this system.
	int status, socket_fd;
	struct addrinfo hints, *servAddr, *temp;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;		// IP version independent.
	hints.ai_socktype = SOCK_STREAM;	// Stream Socket.
	hints.ai_flags = AI_PASSIVE;		// Set My address for me.

	status = getaddrinfo(NULL, port, &hints, &servAddr);
	assert(status==0);

	// Try to create and bind socket to each available address.
	for (temp = servAddr; temp!=NULL; temp = temp->ai_next){
		socket_fd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if (socket_fd == -1)
			continue;

		int optval = 1;
		status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		assert(status!=-1);

		status = bind(socket_fd, temp->ai_addr, temp->ai_addrlen);
		if (status==0){
			if (errno == EACCES)
				printf("Should have root privileges to bind to port : %s\n",port);
			break;
		}

		close(socket_fd);
	}
	return socket_fd;
}

/**
 * Make a socket nonblocking.
 * @param socket : Socket address to set non blocking.
 * @returns void, no need of error checking.
 * 
 * Operations on a nonblocking socket returns immediately
 * without blocking. Specialized mechanisms like poll,
 * select, epoll should be used for monitoring nonblocking
 * sockets events.
 */
void socket_make_nonblocking(int socket)
{
	int flags, s;

	// Get already set flags.
	flags = fcntl(socket, F_GETFL, 0);
	assert(flags!=-1);

	// Add NONBLOCK option to socket and set it.
	flags |= O_NONBLOCK;
	s = fcntl(socket, F_SETFL, flags);
	assert(s!=-1);
}


/**
 * Connects to the server whose ip is `ip` with port `port`.
 * @param ip : Ip address of the server.
 * @param port : Port address of the server.
 * @return Socket address on success or -1 on error.
 * 
 * connect_to_server is a compound function where it takes
 * the ip and port parameter, creates a socket and connect
 * to the server specified in ip. Returns the socket on success
 * and -1 on error.
 */
int socket_connect_to_server(char *ip, char *port)
{
	// Get Address of this system.
	int status, socket_fd = -1;
	struct addrinfo hints, *servAddr, *temp;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;		// IP version independent.
	hints.ai_socktype = SOCK_STREAM;	// Stream Socket.
	//hints.ai_flags = AI_PASSIVE;		// Set My address for me.

	status = getaddrinfo(ip, port, &hints, &servAddr);
	assert(status==0);

	// Try to create and bind socket to each available address.
	for (temp = servAddr; temp!=NULL; temp = temp->ai_next){
		socket_fd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if (socket_fd == -1)
			continue;

		int optval = 1;
		status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		assert(status!=-1);

		status = connect(socket_fd, temp->ai_addr, temp->ai_addrlen);
		if (status==0)
			break;

		close(socket_fd);
	}
	return socket_fd;
}

/**
 * Listen from server socket `server_fd`
 */
inline int socket_listen(int server_fd)
{
	return listen(server_fd, 10);
}


/**
 * Same as accepting connection and making the socket nonblocking.
 * @param server_fd : Server file descriptor from which client should be accepted.
 * @returns accepted client's file descriptor or -1 on error.
 */
int socket_accept_nonblocking(int server_fd)
{
	int client = accept(server_fd, NULL, NULL);
	if (client==-1)
		return -1;
	socket_make_nonblocking(client);
	return client;
}

/**
 * Accepts a connection on server_fd and returns the client socket.
 * Remember that the client socket is not set to be nonblocking.
 * @param server_fd : Server socket from which connection should be accepted.
 * @returns accepted socket connection address.
 * @see socket_accept_nonblocking() function for accepting connection
 * that is nonblocking.
 */
int socket_accept(int server_fd)
{
	int client = accept(server_fd, NULL, NULL);
	if (client==-1)
		return -1;
	return client;
}