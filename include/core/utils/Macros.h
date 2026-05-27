#ifndef GENESIS_CORE_MACROS_H
#define GENESIS_CORE_MACROS_H
#include <cassert>

#define GENESIS_ASSERT(condition, msg)\
assert(condition && msg)

#define GENESIS_DISABLE_COPY_AND_MOVE(className)\
	public:\
		className(const className&) = delete;\
		className(className&&) = delete;\
		className& operator =(const className&) = delete;\
		className& operator =(className&&) = delete;

#endif

#define GENESIS_NAMESPACE_BEGIN namespace genesis {

#define GENESIS_NAMESPACE_END }