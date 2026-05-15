#ifndef GENESIS_CORE_MACROS_H
#define GENESIS_CORE_MACROS_H
#include <cassert>

#define GENESIS_INVALID_ENTITY -1

#define GENESIS_ASSERT(condition, msg)\
assert(condition && msg)

#define GENESIS_DISABLE_COPY_AND_MOVE(className)\
	public:\
		className(const className&) = delete;\
		className(className&&) = delete;\
		className& operator =(const className&) = delete;\
		className& operator =(className&&) = delete;

#endif