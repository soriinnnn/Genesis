#ifndef GENESIS_WINDOW_H
#define GENESIS_WINDOW_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Rect.h>

namespace genesis
{
    enum class WindowStyle
    {
        Windowed,
        Borderless
    };

    struct WindowDesc
    {
        BaseDesc base;
        Rect size{};
        const char* title{};
        WindowStyle style = WindowStyle::Windowed;
    };

    class Window: public Base
    {
    public:
        virtual ~Window() override;

        bool hasFocus() const noexcept;
        void* getHandle() const noexcept;
        Rect getSize() const noexcept;

        virtual void resize(uint32 width, uint32 height) = 0;
        virtual void centerOnScreen() = 0;
        virtual void setPosition(uint32 x, uint32 y) = 0;
        virtual void setStyle(WindowStyle style) = 0;

        void onResize(std::function<void(uint32, uint32)> callback);
        void onFocusChanged(std::function<void(bool)> callback);

        static UniquePtr<Window> create(const WindowDesc& desc);

    protected:
        explicit Window(const WindowDesc& desc);

    protected:
        void* m_handle;
        Rect m_size;
        bool m_hasFocus;
        WindowStyle m_style;

    protected:
        std::function<void(uint32, uint32)> m_onResize;
        std::function<void(bool)> m_onFocusChanged;
    };
}

#endif