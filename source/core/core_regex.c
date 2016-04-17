/**
 * @file core_regex.c
 * @author Kishore
 * @date March 26, 2016, 9:21 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/init.h"
#include "headers/core_memory.h"
#include "headers/core_regex.h"


struct{
	char *string;
	char *regex;
	int string_len;
	int string_parsed;
	int regex_parsed;
	int cap_index;
	memory_get_t *mem;
}_;


enum atom_error{
	PARSE_STRING_OVER,
	PARSE_ERROR
};
typedef struct {
	int success;
	enum atom_error error;
}regex_atom_t;


regex_status_t _parse_regex(regex_cap_t *caps, int n_caps);

char* char_get();
void char_move();

void regex_move();
int regex_get();
int regex_get_i(int i);

regex_atom_t parse_atom();


/**
 * Parses string with regular expression.
 * @param regex
 * @param string
 * @param caps
 * @param n_caps
 * @return 
 */
regex_status_t regex_parse_string(char *regex, char *string, regex_cap_t *caps, int n_caps)
{
	_.regex = regex;
	_.string = string;
	_.string_len = -1;
	_.string_parsed = 0;
	_.regex_parsed = 0;
	_.mem = NULL;
	_.cap_index = 0;
	
	return _parse_regex(caps, n_caps);
}

regex_status_t regex_parse_memory(char *regex, char *string, int len, regex_cap_t *caps, int n_caps)
{
	_.regex = regex;
	_.string = string;
	_.string_len = len;
	_.string_parsed = 0;
	_.regex_parsed = 0;
	_.mem = NULL;
	_.cap_index = 0;
	
	return _parse_regex(caps, n_caps);
}

regex_status_t regex_parse_memoryinstance(char *regex, memory_get_t *mg, regex_cap_t *caps, int n_caps)
{
	_.regex = regex;
	_.string = NULL;
	_.string_len = -1;
	_.string_parsed = 0;
	_.regex_parsed = 0;
	_.mem = mg;
	_.cap_index = 0;

	return _parse_regex(caps, n_caps);
}

regex_status_t _parse_regex(regex_cap_t *caps, int n_caps)
{
	int cap_index = _.cap_index;
	
	regex_status_t status;
	
	int status_defined = FALSE;
	
	regex_atom_t atom_status;
	while (1)
	{
		switch(regex_get())
		{
			case '*':
			{
				if (status_defined)
					return status;
				
				regex_move();
				while ((atom_status = parse_atom()).success)
				{
					char_move();
				}
				int tmp_regex_parsed = _.regex_parsed;
				regex_move();
				if (regex_get()=='\0')
					break;
				_.regex_parsed = tmp_regex_parsed;
				
				if (atom_status.error==PARSE_STRING_OVER)
				{
					status.str_parsed = _.string_parsed;
					status.regex_parsed = _.regex_parsed-1;
					status.n_caps = cap_index+1;
					if (_.mem!=NULL)
						status.cont.mg = *_.mem;
					status.status = REGEX_STRING_UNDERFLOW;
					status_defined = TRUE;
				}
				regex_move();
				break;
			}
			case '+':
			{
				if (status_defined)
					return status;
				
				status.n_caps = cap_index+1;
				status.regex_parsed = _.regex_parsed;
				
				int n_occurences = 0;
						regex_move();


				regex_move();

				while ((atom_status = parse_atom()).success)
				{
					n_occurences++;
					char_move();
				}
				status.str_parsed = _.string_parsed;
				
				if (n_occurences==0)
				{
					if (atom_status.error == PARSE_STRING_OVER)
					{
						if (_.mem!=NULL)
							status.cont.mg = *_.mem;

						status.status = REGEX_STRING_UNDERFLOW;
						return status;
					}
					else if (atom_status.error==PARSE_ERROR)
					{
						status.status = REGEX_PARSE_ERROR;
						return status;
					}
					else
					{
						ansi_error("This condition should never occur.\n");
						exit(1);
					}
				}
				if (char_get()==NULL)
				{
					if (_.mem!=NULL)
						status.cont.mg = *_.mem;
					status.status = REGEX_STRING_UNDERFLOW;
					status_defined = TRUE;
				}
				regex_move();
				break;
			}
			case '(':
			{
				regex_move();
				if (cap_index<n_caps){
					caps[cap_index].index = _.string_parsed;
				}
				break;
			}
			case ')':
			{
				regex_move();
				if (cap_index<n_caps)
				{
					caps[cap_index].len = _.string_parsed - caps[cap_index].index;
					cap_index++;
				}
				break;
			}
			case '\0':
			{
				status.regex_parsed = _.regex_parsed;
				status.str_parsed = _.string_parsed;
				status.n_caps = cap_index+1;
				
				if ((char_get()==NULL) || (*(char_get())=='\0'))
				{
					if (_.mem!=NULL)
						status.cont.mg = *_.mem;
					status.status = REGEX_PARSE_SUCCESS;
				}
				else
				{
					if (_.mem!=NULL)
						status.cont.mg = *_.mem;
					status.status = REGEX_STRING_OVERFLOWN;
				}
				return status;
			}
			default:
			{
				if (status_defined)
					return status;
				if ((atom_status = parse_atom()).success)
				{
					regex_move();
					char_move();
					break;
				}
				
				// Return failure status.
				status.regex_parsed = _.regex_parsed;
				status.str_parsed = _.string_parsed;
				status.n_caps = cap_index+1;
				if (atom_status.error==PARSE_STRING_OVER){
					if (_.mem!=NULL)
						status.cont.mg = *_.mem;
					status.status = REGEX_STRING_UNDERFLOW;
				}
				else
					status.status = REGEX_PARSE_ERROR;
				return status;
			}
		}
	}
}


char* char_get()
{
	if (_.string!=NULL)
	{
		if (_.string_len!=-1)
		{
			if (_.string_parsed==_.string_len)
				return NULL;
			return _.string+_.string_parsed;
		}
		else if (_.string[_.string_parsed]=='\0')
			return NULL;
		return _.string+_.string_parsed;
	}
	else
	{
		assert(_.mem!=NULL);

		if (_.mem->data==NULL)
		{
			return NULL;
		}
		return _.mem->data+_.mem->index;
	}
}

void char_move()
{
	if (_.string!=NULL)
	{
		_.string_parsed++;
	}
	else
	{
		assert(_.mem->data!=NULL);
		
		_.mem->index++;
		_.string_parsed++;
		assert(_.mem->index<=_.mem->len);
		if (_.mem->index==_.mem->len)
		{
			memory_get(_.mem);
		}
	}
}

void regex_move()
{
	switch(_.regex[_.regex_parsed])
	{
		case '<':{
			int i=_.regex_parsed;
			for (i=0; (_.regex[i]!='>') && (_.regex[i]!='\0'); i++);
			if (_.regex[i]=='\0'){
				ansi_error("Error in regex %s : < should always be followed by >.\n", _.regex);
				exit(1);
			}
			_.regex_parsed = i+1;
			break;
		}
		case '{':{
			int i;
			for (i=_.regex_parsed; (_.regex[i]!='}') && (_.regex[i]!='\0'); i++);
			if (_.regex[i]=='\0'){
				ansi_error("Error in regex %s : { should always be followed by }.\n", _.regex);
				exit(1);
			}
			_.regex_parsed = i+1;
			break;
		}
		case '!':{
			_.regex_parsed++;
			regex_move();
			break;
		}
		default:{
			assert(_.regex[_.regex_parsed]!='\0');
			_.regex_parsed++;
		}
	}
}

int regex_get()
{
	return _.regex[_.regex_parsed];
}

int regex_get_i(int i)
{
	return _.regex[_.regex_parsed+i];
}

struct {
	char class[50];
	int (*func)(int);
}classes[5]={
	{
		.class = "<digit>",
		.func = &isdigit
	},
	{
		.class = "<alpha>",
		.func = &isalpha
	},
	{
		.class = "<upper>",
		.func = &isupper
	},
	{
		.class = "<lower>",
		.func = &islower
	},
	{
		.class = "<alnum>",
		.func = &isalnum
	}
};

regex_atom_t parse_atom()
{
	regex_atom_t status;
	status.error = PARSE_ERROR;

	if (char_get()==NULL)
	{
		status.success = FALSE;
		status.error = PARSE_STRING_OVER;
		return status;
	}
	char ch = *char_get();
	
	switch(regex_get())
	{
		case '{':{
			int i;
			int tmp_regex_index = _.regex_parsed;
			_.regex_parsed++;
			for (i=1; regex_get_i(i)!='\0' && regex_get_i(i)!='}'; i++)
			{
				if (regex_get_i(i)=='<')
				{
					_.regex_parsed += i;
					regex_atom_t ret = parse_atom();
					if (ret.success)
					{
						_.regex_parsed = tmp_regex_index;
						status.success = TRUE;
						return status;
					}
					regex_move();
				}
				else if (ch==regex_get_i(i))
				{
					_.regex_parsed = tmp_regex_index;
					status.success = TRUE;
					return status;
				}
			}
			if (regex_get_i(i)=='\0')
			{
				ansi_error("{ should always be followed by }. String terminated abruptly\n");
				exit(1);
			}
			_.regex_parsed = tmp_regex_index;
			status.success = FALSE;
			status.error = PARSE_ERROR;
			return status;
		}
		case '<':{
			char *regex = _.regex+_.regex_parsed;
			int i=0;
			for (i=0; i<5; i++)
			{
				if (strstr(regex, classes[i].class)==regex)
				{
					if (!classes[i].func(ch))
					{
						status.success = TRUE;
						return status;
					}
					status.success = FALSE;
					status.error = PARSE_ERROR;
					return status;
				}
			}
			ansi_error("Invalid character class present in regex : %s\n", _.regex);
			exit(1);
		}
		case '!':{
			_.regex_parsed++;
			status = parse_atom();
			_.regex_parsed--;
			if (status.success)
			{
				status.success = !status.success;
				status.error = PARSE_ERROR;
				return status;
			}
			if (status.error==PARSE_ERROR)
			{
				status.success = !status.success;
				return status;
			}
			return status;
		}
		default:{
			if (regex_get()==ch)
			{
				status.success = TRUE;
				return status;
			}
			status.success = FALSE;
			status.error = PARSE_ERROR;
			return status;
		}
	}
}


void regex_printStatusDetails(regex_status_t status)
{
	switch (status.status)
	{
		case REGEX_PARSE_ERROR:
		{
			ansi_sinfo("Error in parsing regex.\n");
			break;
		}
		case REGEX_PARSE_SUCCESS:
		{
			ansi_sinfo("Success.\n");
			break;
		}
		case REGEX_STRING_MAY_EXTEND:
		{
			ansi_sinfo("Success, but string can still continue.\n");
			break;
		}
		case REGEX_STRING_UNDERFLOW:
		{
			ansi_sinfo("Regex not parsed completely. String incomplete.\n");
			break;
		}
		case REGEX_STRING_OVERFLOWN:
		{
			ansi_sinfo("Regex parsed successfully, but string is not yet complete.\n");
			break;
		}
	}
}