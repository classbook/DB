/**
 * @file core_init.h
 * @author Kishore
 * @date March 20, 2016, 12:59 PM 
 */

#include "common/headers/stdheaders.h"


#ifndef UNIQUE_CORE_DEFINITIONS_IDENTIFIER
	#define UNIQUE_CORE_DEFINITIONS_IDENTIFIER

	/**
	 * ==================================
	 * ANSI RELATED DEFINITIONS GOES HERE
	 * ==================================
	 */
	enum colors {
		C_BLACK,
		C_RED,
		C_GREEN,
		C_YELLOW,
		C_BLUE,
		C_MAGENTA,
		C_CYAN,
		C_WHITE,
		C_DEFAULT = 9
	};

	enum params {
		A_BOLD = 1u << 0
	};

	
	#define ansi_bold_text(...)\
		{\
			ansi_bold();\
			printf(__VA_ARGS__);\
			ansi_default();\
		}

	#define ansi_uline_text(...)\
		{\
			ansi_uline();\
			printf(__VA_ARGS__);\
			ansi_default();\
		}

	#define ansi_uline_bold_text(...)\
		{\
			ansi_bold();\
			ansi_uline();\
			printf(__VA_ARGS__);\
			ansi_default();\
		}

	#define ansi_fg_bg_text(fg, bg, ...)\
		{\
			ansi_fg_bg(fg,bg);\
			printf( __VA_ARGS__ );\
			ansi_default();\
			fflush(stdout);\
		}


	#define ansi_fg_bg_textln(a, b, ...)\
		{\
			ansi_fg_bg(a,b);\
			printf( __VA_ARGS__ );\
			ansi_default();\
			printf("\n");\
			fflush(stdout);\
		}

	#define ansi_success(...)\
		{\
			ansi_bold();\
			ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\nSuccess : " __VA_ARGS__);\
			fflush(stdout);\
		}

	#define ansi_error(...)\
		{\
			ansi_fg_bg_text(C_RED, C_DEFAULT, "%s:%d: ", __FILE__, __LINE__);\
			ansi_fg_bg_text(C_RED, C_DEFAULT, __VA_ARGS__);\
			fflush(stdout);\
		}

	#define ansi_sinfo(...)\
		{\
			ansi_clear_line();\
			ansi_fg_bg_text(C_MAGENTA, C_DEFAULT, __VA_ARGS__);\
		}

	#define ansi_info(...)\
		{\
			ansi_clear_line();\
			ansi_fg_bg_text(C_GREEN, C_DEFAULT, __VA_ARGS__);\
		}
	
	/**
	 * ======================================
	 * SOCKET RELATED Definitions GOES HERE
	 * ======================================
	 */
	
	
	
	/**
	 * =========================
	 * REQUEST CONTROLLER MODULE 
	 * =========================
	 */
	typedef struct request_struct{
		int fd;
		int mem_index;	// Implementation dependent
		int data;
		int isempty;
		int _next;
	}request_t;
	
	typedef struct request_get_struct{
		int req_id;
		int fd;
		int data;
		int mem_index;
		uint32_t events;
	}request_get_t;
	
	#define REQUEST_MAXREQUESTS 1000

	/**
	 * ========================
	 *      MEMORY MODULE
	 * ========================
	 */
	#define MEMORY_UNIT_MAXSIZE 256
	#define MEMORY_N_UNITS 1000
	#define MEMORY_N_INSTANCES REQUEST_MAXREQUESTS

	typedef struct memory_struct{
		int sindex;
		int filled;
		char data[MEMORY_UNIT_MAXSIZE];
		struct memory_struct *next;
	}memory_t;
	
	
	typedef struct memory_instance_struct{
		memory_t *start;
		memory_t *end;
		int free;
		int t_len;
		int next;
	}memory_instance_t;
	
	
	typedef struct{
		struct memory_struct *_memory;
		int index;
		int len;
		char *data;
	}memory_get_t;
#endif