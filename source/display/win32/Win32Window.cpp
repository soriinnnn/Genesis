#include <display/win32/Win32Window.h>
#include <filesystem>

#define WINDOW_CLASS_NAME L"GENESIS_WINDOW_CLASS"
#define ICON_EXTENSION ".ico"
#define CHECK_INVALID_WINDOW_POINTER(wnd, hwnd, msg, wparam, lparam) \
if (!(wnd)) {\
    return DefWindowProc((hwnd), (msg), (wparam), (lparam));\
}

using namespace genesis;
using namespace std;

static String getPathExtension(const char* path);
static WString getAbsolutePath(const char* path);
static WNDCLASSEX getWindowClass(HINSTANCE instance, LPCTSTR className, WNDPROC windowProc);
static RECT createWindowRect(UINT width, UINT height, DWORD style);
static DWORD getWindowStyle(WindowStyle style);

Win32Window::Win32Window(const WindowDesc& desc): Window(desc), m_instance{GetModuleHandle(nullptr)}, m_cursor{LoadCursor(nullptr, IDC_ARROW)}, m_icon{nullptr}
{
    WNDCLASSEX wndClass;
    if (!GetClassInfoEx(m_instance, WINDOW_CLASS_NAME, &wndClass)) {
        wndClass = getWindowClass(m_instance, WINDOW_CLASS_NAME, windowProc);
        if (!RegisterClassEx(&wndClass)) {
            GENESIS_LOG_THROW_ERROR("RegisterClassEx failed.\nError code: 0x{:08X}", GetLastError());
        }
    }
    open();
    setIcon(desc.icon);
}

Win32Window::~Win32Window()
{
    close();
}

Rect Win32Window::getScreenSize() const
{
    return Rect{
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN)
    };
}

void Win32Window::setPosition(const Point& position)
{
    BOOL result = SetWindowPos(
        static_cast<HWND>(m_handle),
        nullptr,
        position.x,
        position.y,
        0,
        0,
        SWP_NOSIZE | SWP_NOZORDER
    );
    if (!result) {
        GENESIS_LOG_ERROR("SetWindowPos failed.\nError code: 0x{:08X}", GetLastError());
        return;
    }
}

void Win32Window::setSize(const Rect& size)
{
    RECT wndRect = createWindowRect(size.width(), size.height(), getWindowStyle(m_style));
    BOOL result = SetWindowPos(
        static_cast<HWND>(m_handle),
        nullptr,
        0,
        0,
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top,
        SWP_NOMOVE | SWP_NOZORDER
    );
    if (!result) {
        GENESIS_LOG_ERROR("SetWindowPos failed.\nError code: 0x{:08X}", GetLastError());
        return;
    }
}

void Win32Window::setStyle(WindowStyle style)
{
    HWND hwnd = static_cast<HWND>(m_handle);
    DWORD wndStyle = getWindowStyle(style);
    RECT wndRect = createWindowRect(m_size.width(), m_size.height(), wndStyle);

    SetWindowLongPtr(hwnd, GWL_STYLE, wndStyle);
    BOOL result = SetWindowPos(
        hwnd,
        nullptr,
        0, 
        0, 
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top,
        SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
    );
    if (!result) {
        GENESIS_LOG_ERROR("SetWindowPos failed.\nError code: 0x{:08X}", GetLastError());
        return;
    }
    m_style = style;
}

void Win32Window::setTitle(const char* title)
{
    WString wndTitle = WString(title, title + strlen(title));
    if (!SetWindowText(static_cast<HWND>(m_handle), wndTitle.c_str())) {
        GENESIS_LOG_ERROR("SetWindowText failed. Error: 0x{:08X}", GetLastError());
    }
    m_title = String{title};
}

void Win32Window::setIcon(const char* path)
{
    if (!path) {
        return;
    }

    String extension = getPathExtension(path);
    if (extension != ICON_EXTENSION) {
        GENESIS_LOG_ERROR("Invalid icon format: \"{}\". Only \"{}\" files are supported.", extension, ICON_EXTENSION);
        return;
    }

    HICON icon = static_cast<HICON>(LoadImage(
        nullptr,
        getAbsolutePath(path).c_str(),
        IMAGE_ICON,
        0,
        0,
        LR_DEFAULTSIZE | LR_LOADFROMFILE
    ));

    if (!icon) {
        GENESIS_LOG_ERROR("LoadIcon failed. Error: 0x{:08X}", GetLastError());
        return;
    }

    HWND hwnd = static_cast<HWND>(m_handle);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));

    if (m_icon) {
        DestroyIcon(m_icon);
    }
    m_icon = icon;
}

void Win32Window::show()
{
    if (isVisible()) {
        return;
    }
    ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
}

void Win32Window::hide()
{
    if (!isVisible()) {
        return;
    }
    ShowWindow(static_cast<HWND>(m_handle), SW_HIDE);
}

void Win32Window::center()
{
    Rect screen = getScreenSize();
    setPosition(Point{
        (screen.width() - m_size.width()) / 2, 
        (screen.height() - m_size.height()) / 2
    });
}

void Win32Window::open()
{
    WString title{m_title.begin(), m_title.end()};
    DWORD style = getWindowStyle(m_style);
    RECT size = createWindowRect(m_size.width(), m_size.height(), style);

    m_handle = CreateWindowEx(
        0,
        WINDOW_CLASS_NAME,
        title.c_str(),
        style,
        m_position.x,
        m_position.y,
        size.right - size.left,
        size.bottom - size.top,
        nullptr,
        nullptr,
        m_instance,
        this
    );
    if (!m_handle) {
        GENESIS_LOG_THROW_ERROR("CreateWindowEx failed.\nError code: 0x{:08X}", GetLastError());
    }
}

void Win32Window::close()
{
    if (!m_handle) {
        return;
    }
    if (m_icon) {
        DestroyIcon(m_icon);
    }
    if (!DestroyWindow(static_cast<HWND>(m_handle))) {
        GENESIS_LOG_THROW_ERROR("DestroyWindow failed.\nError code: 0x{:08X}", GetLastError());
    }
    m_handle = nullptr;
}

LRESULT CALLBACK Win32Window::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
        case WM_CREATE: 
            return onCreate(hwnd, msg, wparam, lparam);
        case WM_DESTROY:
            return onDestroy(hwnd, msg, wparam, lparam);
        case WM_CLOSE: 
            return onClose(hwnd, msg, wparam, lparam);
        case WM_MOVE:
            return onMove(hwnd, msg, wparam, lparam);
        case WM_SIZE:
            return onSize(hwnd, msg, wparam, lparam);  
        case WM_SHOWWINDOW:
            return onShowWindow(hwnd, msg, wparam, lparam);
        case WM_ACTIVATE:
            return onActivate(hwnd, msg, wparam, lparam);
        case WM_SETCURSOR:
            return onSetCursor(hwnd, msg, wparam, lparam);
        default: 
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

LRESULT Win32Window::onCreate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lparam);
    Win32Window* wnd = static_cast<Win32Window*>(cs->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
    return 0;
}

LRESULT Win32Window::onDestroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT Win32Window::onClose(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    CHECK_INVALID_WINDOW_POINTER(wnd, hwnd, msg, wparam, lparam);
    wnd->close();
    return 0;
}

LRESULT Win32Window::onMove(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    CHECK_INVALID_WINDOW_POINTER(wnd, hwnd, msg, wparam, lparam);

    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        wnd->m_position = Point{rect.left, rect.top};
    }

    return 0;
}

LRESULT Win32Window::onSize(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    CHECK_INVALID_WINDOW_POINTER(wnd, hwnd, msg, wparam, lparam);

    UINT width = LOWORD(lparam);
    UINT height = HIWORD(lparam);

    switch (wparam) {
        case SIZE_RESTORED: {
            wnd->m_isVisible = true;
            break;
        }
        case SIZE_MINIMIZED: {
            wnd->m_isVisible = false;
            break;
        }
        case SIZE_MAXIMIZED: {
            wnd->m_isVisible = true;
            break;
        }/*
        case SIZE_MAXHIDE: {
            break;
        }
        case SIZE_MAXSHOW: {
            break;
        }*/
    }

    if (wparam != SIZE_MINIMIZED) {
        wnd->m_size = Rect{static_cast<int32>(width), static_cast<int32>(height)};
        if (wnd->m_onResize) {
            wnd->m_onResize(wnd->m_size);
        }
    }
    
    return 0;
}

LRESULT Win32Window::onShowWindow(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    CHECK_INVALID_WINDOW_POINTER(wnd, hwnd, msg, wparam, lparam);

    bool visible = (wparam == TRUE);
    wnd->m_isVisible = visible;
    
   /*switch (lparam) {
        case SW_PARENTCLOSING: {
            break;
        }
        case SW_PARENTOPENING: {
            break;
        }
        case SW_OTHERZOOM: {
            break;
        }
        case SW_OTHERUNZOOM: {
            break;
        }
    }*/

    return 0;
}

LRESULT Win32Window::onActivate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    CHECK_INVALID_WINDOW_POINTER(wnd, hwnd, msg, wparam, lparam);

    WORD state = LOWORD(wparam);
    bool hasFocus = (state != WA_INACTIVE);

    wnd->m_hasFocus = hasFocus;
    if (wnd->m_onFocusChanged) {
        wnd->m_onFocusChanged(hasFocus);
    }

    return 0;
}

LRESULT Win32Window::onSetCursor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    CHECK_INVALID_WINDOW_POINTER(wnd, hwnd, msg, wparam, lparam);

    WORD hitTest = LOWORD(lparam);
    if (hitTest == HTCLIENT) {
        SetCursor(wnd->m_cursor);
        return TRUE;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

/* STATIC FUNCTION DEFINITIONS */

String getPathExtension(const char* path)
{
    GENESIS_ASSERT(path, "Path is null.");
    return std::filesystem::path(path).extension().generic_string();
}

WString getAbsolutePath(const char* path)
{
    GENESIS_ASSERT(path, "Path is null.");
    return std::filesystem::absolute(path).generic_wstring();
}

WNDCLASSEX getWindowClass(HINSTANCE instance, LPCTSTR className, WNDPROC windowProc)
{
    WNDCLASSEX wndClass = {};

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = 0;
    wndClass.lpfnWndProc = windowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = instance;
    wndClass.hIcon = nullptr;
    wndClass.hbrBackground = nullptr;
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = className;
    wndClass.hIconSm = nullptr;

    return wndClass;
}

RECT createWindowRect(UINT width, UINT height, DWORD style)
{
    RECT wndRect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    AdjustWindowRect(&wndRect, style, false);
    return wndRect;
}

DWORD getWindowStyle(WindowStyle style)
{
    switch (style) {
    case WindowStyle::Windowed:
        return WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    case WindowStyle::Borderless:
        return WS_OVERLAPPED | WS_VISIBLE;
    default:
        return WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    }
}