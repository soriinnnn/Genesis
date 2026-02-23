#ifndef GENESIS_BASE_H
#define GENESIS_BASE_H
#include <core/Logger.h>

namespace genesis 
{
    struct BaseDesc
    {
        Logger& logger;
    };

    class Base 
    {
    public:
        virtual ~Base();
        Logger& getLogger() const noexcept;

    protected:
        explicit Base(const BaseDesc& desc);

        Base(const Base&) = delete;
        Base(Base&&) = delete;
        Base& operator =(const Base&) = delete;
        Base& operator =(Base&&) = delete;

    protected:
        Logger& m_logger;
    };
}

#endif