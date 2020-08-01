//
// Utility.c - Misc. Utility Functions
//
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "defs.h"

/*
=====================
va_append
=====================
Append char arrays using va_args and a buffer.
*/
void va_append(char* buf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
}