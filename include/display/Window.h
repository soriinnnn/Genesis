#ifndef GENESIS_WINDOW_H
#define GENESIS_WINDOW_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Rect.h>
#include <math/Point.h>

GENESIS_NAMESPACE_BEGIN

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

    const void* getHandle() const noexcept;
    void* getHandle() noexcept;

    Point getPosition() const noexcept;
    Rect getSize() const noexcept;
    WindowStyle getStyle() const noexcept;
    const char* getTitle() const noexcept;
    virtual Rect getScreenSize() const = 0;

    virtual void setPosition(const Point& position) = 0;
    virtual void setSize(const Rect& size) = 0;
    virtual void setStyle(WindowStyle style) = 0;
    virtual void setTitle(const char* title) = 0;

    bool isVisible() const noexcept;
    bool hasFocus() const noexcept;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void center() = 0;

    void onResize(std::function<void(Rect)> callback);
    void onFocusChanged(std::function<void(bool)> callback);

    static UniquePtr<Window> create(const WindowDesc& desc);

protected:
    explicit Window(const WindowDesc& desc);

protected:
    void* m_handle;
    Point m_position;
    Rect m_size;
    WindowStyle m_style;
    String m_title;
    bool m_isVisible;
    bool m_hasFocus;

protected:
    std::function<void(Rect)> m_onResize;
    std::function<void(bool)> m_onFocusChanged;
};

GENESIS_NAMESPACE_END

#endif