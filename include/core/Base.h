#ifndef GENESIS_BASE_H
#define GENESIS_BASE_H
#include <core/Logger.h>
#include <core/utils/LogUtils.h>
#include <core/utils/Macros.h>

GENESIS_NAMESPACE_START

struct BaseDesc
{
    Logger& logger;
};

class Base
{
GENESIS_DISABLE_COPY_AND_MOVE(Base)
public:
    virtual ~Base();
    Logger& getLogger() const noexcept;

protected:
    explicit Base(const BaseDesc& desc);

protected:
    Logger& m_logger;
};
    
GENESIS_NAMESPACE_END

#endif