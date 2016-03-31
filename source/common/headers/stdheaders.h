/**
 * @file headers.h
 * @author Kishore
 * @date March 20, 2016, 12:51 PM 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <inttypes.h>

#include <netdb.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE 0
#endif