#include "io.h"

#ifndef _STRINGS_H
#define _STRINGS_H

int strcmp( const char *s1, const char *s2 );
int strncmp( const char *s1, const char *s2, int n );
void strwrite(sender_func_type output, const char * string);
void print_number(sender_func_type output, int num);

#endif