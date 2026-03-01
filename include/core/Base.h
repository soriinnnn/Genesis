#ifndef GENESIS_BASE_H
#define GENESIS_BASE_H
#include <core/Logger.h>
#include <core/utils/Macros.h>
#include <core/utils/LogMacros.h>

namespace genesis 
{
    struct BaseDesc
    {
        Logger& logger;
    };

    class Base
    {
    GENESIS_DISABLE_COPY_AND_MOVE(Base)
    public:
        virtual ~Base();
        virtual Logger& getLogger() const noexcept final;
        
    protected:
        explicit Base(const BaseDesc& desc);

    protected:
        Logger& m_logger;
    };
}

#endif