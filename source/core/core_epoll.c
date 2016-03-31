/**
 * @file lib_epoll.c
 * @author Kishore
 * @date June 25, 2015, 12:00 PM
 * 
 * @brief Library for wrapping epoll functionality
 * with simple to use API.
 */

#include "headers/core_epoll.h"
#include "headers/core_ansi.h"


static struct epoll_event * _epoll_interest_list(char *);
static struct epoll_event * _epoll_interest_list(char *interest)
{
	static struct epoll_event ev;
	ev.events = 0;
	ev.events |= EPOLLET;
	if (interest[0]=='r' || interest[1]=='r')
		ev.events |= EPOLLIN;
	if (interest[0]=='w' || interest[1]=='w')
		ev.events |= EPOLLOUT;
	return &ev;
}

/**
 * Adds a file descriptor to the epoll interest list.
 * @param epfd : epoll instance.
 * @param fd : file descriptor that is to be added to the interest list.
 * @param interest : string of r and w which denotes read or write interest.
 * @param create_memory : boolean value that decides whether to associate
 * memory instance with epoll member or not.
 * @returns 0 on success and -1 on error.
 * 
 * The epoll_member identifier is stored in ev.data.fd field. To
 * retrieve the identifier later access ev.data.fd field.
 */
inline int epoll_add(int epfd, int fd, char *interest, int data)
{
	struct epoll_event *ev = _epoll_interest_list(interest);
	
	// Store epoll_member index in data field of epoll_event.
	ev->data.fd = data;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev)==-1){
		ansi_error("Couldn't add file descriptor to epoll list\n");
		return -1;
	}
	return 1;
}

/**
 * Modifies the interest list of file descriptor in epoll instance.
 * @param epfd : the epoll instance where the file descriptors are monitored.
 * @param ep_index : epoll_member identifier whose interest list should be
 * modified.
 * @param interest : string combination of r and w.
 * @returns 0 on success and -1 on failure.
 * 
 * This call modifies the interest list of epoll_member so that epoll
 * instance only watches for modified interest list of epoll member.
 */
inline int epoll_modify(int epfd, int fd, int data, char *interest)
{
	struct epoll_event *ev = _epoll_interest_list(interest);
	ev->data.fd = data;
	return epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
}

/**
 * Removes the epoll_member from list of monitoring in epoll.
 * @param epfd : epoll instance where the file descriptors are monitored.
 * @param ep_index : epoll_member identifier that should be removed.
 * @returns 0 on success and -1 on failure.
 * 
 * Removes the epoll_member from epoll instance.
 */
inline int epoll_remove(int epfd, int fd)
{
	int status = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	if (status==-1)
		ansi_error("Failed to remove file descriptor : %d from epoll instance.\n", fd);
	return status;
}


/**
 * Check if read event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if read event has occured else returns 0.
 * 
 * Make sure that parameter events is epoll_event.events value, not epoll_event.
 * Checks if an read event occurred.
 */
inline int epoll_can_read(uint32_t events)
{
	if ((events & EPOLLIN) || (events & EPOLLPRI || (events & EPOLLRDBAND) || (events & EPOLLRDNORM)))
		return 1;
	return 0;
}

/**
 * Check if write event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if write event has occured else returns 0.
 * 
 * Make sure that parameter events is epoll_event.events value, not epoll_event.
 * Checks if a write event occurred.
 */
inline int epoll_can_write(uint32_t events)
{
	if ((events & EPOLLOUT) || (events & EPOLLWRNORM) || (events & EPOLLWRBAND))
		return 1;
	return 0;
}

/**
 * Check if shutdown event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if shutdown event has occured else returns 0.
 * 
 * Make sure that parameter events is epoll_event.events value, not epoll_event.
 * Checks if shutdown event occurred.
 */

inline int epoll_is_shutdown(uint32_t events)
{
	if ((events & EPOLLRDHUP) || (events & EPOLLHUP))
		return 1;
	return 0;
}

/**
 * Check if error event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if error event has occured else returns 0.
 * 
 * Make sure that parameter events is epoll_event.events value, not epoll_event.
 * Checks if an error event occurred.
 */

inline int epoll_has_error(uint32_t events)
{
	if (events & EPOLLERR)
		return 1;
	return 0;
}