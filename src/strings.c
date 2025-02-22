#import "../include/strings.h"
#include "../include/io.h"

int strcmp( const char *s1, const char *s2 ) {
    while(*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp( const char *s1, const char *s2, int n ) {
    int i = 0;
    while(*s1 && (*s1 == *s2) && i < n)
    {
        s1++;
        s2++;
        i++;
    }
    if(i == n) {
        return 0;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void strwrite(sender_func_type output, const char * string) {
    int i = 0;
    while(string[i] != '\0') {
        output(string[i++]);
    }
}