#ifndef GENESIS_WINDOW_H
#define GENESIS_WINDOW_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Rect.h>

namespace genesis
{
    struct WindowDesc
    {
        BaseDesc base;
        Rect size{};
#ifdef _WIN32
        const wchar_t* title{};
#else
        const char* title{};
#endif
    };

    class Window: public Base
    {
    public:
        virtual ~Window() override;

        void* getHandle() const noexcept;
        Rect getSize() const noexcept;

        virtual void resize(uint32 width, uint32 height) = 0;
        void onResize(std::function<void(uint32, uint32)> callback);

        static SharedPtr<Window> create(const WindowDesc& desc);

    protected:
        explicit Window(const WindowDesc& desc);

    protected:
        void* m_handle;
        Rect m_size;
        std::function<void(uint32, uint32)> m_onResizeCallback;
    };
}

#endif