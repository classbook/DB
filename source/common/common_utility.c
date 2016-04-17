/**
 * @file common_utility.c
 * @author Kishore
 * @date April 16, 2016, 11:27 AM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/common_utility.h"
void print_trace(void)
{
	void *array[100];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 100);
	strings = backtrace_symbols (array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
	   printf ("%s\n", strings[i]);

	free (strings);
}