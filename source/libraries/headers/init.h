/**
 * @file lib_init.h
 * @author Kishore
 * @date March 19, 2016, 11:35 PM 
 */

#ifndef UNIQUE_LIB_DEFINITIONS_IDENTIFIER
	#define UNIQUE_LIB_DEFINITIONS_IDENTIFIER
	
	/**
	 * ==================================
	 * ANSI RELATED DEFINITIONS GOES HERE
	 * ==================================
	 */

	typedef struct {
		int memory_index;
		int is_parsed;
		struct {
			int length;
			int content_length;
			memory_pointer_t _end;
			int _end_l;
		}headers;
		memory_pointer_t write_end;
	}http_request_t;
#endif