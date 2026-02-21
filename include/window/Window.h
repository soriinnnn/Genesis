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
        explicit Window(const WindowDesc& desc);
        virtual ~Window() override;

        virtual Rect getSize() const = 0;
        virtual void resize(uint32 width, uint32 height) = 0;

    protected:
        void* m_handle;
        Rect m_size;

        friend class Display;
    };
}

#endif