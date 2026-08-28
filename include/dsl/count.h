#pragma once
#include <lua.h>

void DSL_count_init(struct lua_State *L);

extern uint8_t countStep;
extern uint8_t countStartingPoint; // used on NG
