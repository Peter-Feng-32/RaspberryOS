#import "../../include/utils/strings.h"
#include "../../include/utils/io.h"

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


void print_number(sender_func_type output, int num) {
    if (num == 0) {
        output('0');
        return;
    }

    char buffer[10];
    int i = 0;

    if (num < 0) {
        output('-');
        num = -num;
    }

    // Convert number to string in reverse order
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Output the digits in correct order
    while (--i >= 0) {
        output(buffer[i]);
    }
}
