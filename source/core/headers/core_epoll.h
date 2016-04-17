/**
 * @file lib_epoll.h
 * @author Kishore
 * @date June 25, 2015, 12:01 PM 
 */

#include "common/headers/init.h"

inline int epoll_add(int epfd, int fd, char *interest, int data);
inline int epoll_modify(int epfd, int fd, int data, char *interest);
inline int epoll_remove(int epfd, int fd);
inline int epoll_get(struct epoll_event ev);

inline int epoll_can_read(uint32_t events);
inline int epoll_can_write(uint32_t events);
inline int epoll_is_shutdown(uint32_t events);
inline int epoll_has_error(uint32_t events);