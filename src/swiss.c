#include "swiss.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

const char *vFormat(const char *fmt, ...)
{
    va_list args, args2;
    va_start(args, fmt);
    va_copy(args2, args);
    size_t sz = vsnprintf(0, 0, fmt, args2) + 1;
    va_end(args2);
    assert(sz > 0 && "vsnprintf() failed");

    char result[sz] = {};
    vsnprintf(result, sz, fmt, args);
    va_end(args);
    return strdup(result);
}
