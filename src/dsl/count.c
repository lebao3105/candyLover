#include "dsl/count.h"
#include "dsl/dsl.h"
#include <lauxlib.h>
#include <types.h>

uint8_t countStep = 1;
uint8_t countStartingPoint = 1;

#define SET_INT_IMPL(name, variable)    \
int set_## name(struct lua_State *L)    \
{                                       \
    ASSERT_IN_SCOPE(count);             \
    WARN_TOO_MANY_ARGS(1, set_## name); \
    variable = DSL_set_integer(L);      \
    return 0;                           \
}

SET_INT_IMPL(count_step, countStep)
SET_INT_IMPL(starting_point, countStartingPoint)
#undef SET_INT_IMPL

void DSL_count_init(struct lua_State *L)
{
    REGISTER_C_FUNCTION(set_count_step);
    REGISTER_C_FUNCTION(set_starting_point);
}
