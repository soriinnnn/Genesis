#ifndef GENESIS_WINDOW_H
#define GENESIS_WINDOW_H
#include <core/Base.h>
#include <core/Core.h>
#include <core/Common.h>

namespace genesis 
{
    class Window: public Base 
    {
    public:
        Window(const WindowDesc& desc);
        virtual ~Window() override;

    protected:
        void* m_handle;
        Rect m_size;
    };
}

#endif