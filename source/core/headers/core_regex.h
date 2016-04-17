/**
 * @file core_regex.h
 * @author Kishore
 * @date March 27, 2016, 9:19 PM 
 */

#include "common/headers/init.h"

regex_status_t regex_parse_string(char *regex, char *string, regex_cap_t *caps, int n_caps);
regex_status_t regex_parse_memory(char *regex, char *string, int len, regex_cap_t *caps, int n_caps);
regex_status_t regex_parse_memoryinstance(char *regex, memory_get_t *mg, regex_cap_t *caps, int n_caps);

void regex_printStatusDetails(regex_status_t status);