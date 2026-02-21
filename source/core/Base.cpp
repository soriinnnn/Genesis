#include <core/Base.h>

using namespace genesis;

Base::Base(const BaseDesc& desc): m_logger(desc.logger) {}

Base::~Base() {}

Logger& Base::getLogger() const noexcept
{
	return m_logger;
}
