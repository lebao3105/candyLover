#include "swiss.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

const char *vFormat(const char *fmt, va_list args)
{
    int sz = vsprintf(nullptr, fmt, args);
    assert(sz > 0 && "vFormat() failed");

    char result[sz] = {};
    vsnprintf(result, sz, fmt, args);
    return strdup(result);
}
