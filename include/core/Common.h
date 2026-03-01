#ifndef GENESIS_COMMON_H
#define GENESIS_COMMON_H

namespace genesis 
{
	struct BinaryData
	{
		const void* m_data{};
		size_t dataSize{};
	};
}

#define GENESIS_DISABLE_COPY_AND_MOVE(className)\
	protected:\
		className(const className&) = delete;\
		className(className&&) = delete;\
		className& operator =(const className&) = delete;\
		className& operator =(className&&) = delete;

#endif