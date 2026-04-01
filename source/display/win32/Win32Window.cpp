#include <display/win32/Win32Window.h>

#define BASE_WINDOW_CLASS_NAME L"GenesisWindow"

using namespace genesis;
using namespace std;

static RECT createWindowRect(UINT width, UINT height, DWORD style);
static ATOM createWindowClass(LPCTSTR className, WNDPROC windowProc);
static DWORD getWindowStyle(WindowStyle style);
static wstring createWindowClassName(void* instance);

Win32Window::Win32Window(const WindowDesc& desc): Window(desc)
{
    wstring title{desc.title, desc.title + strlen(desc.title)};

    DWORD style = getWindowStyle(m_style);
    RECT wndRect = createWindowRect(m_size.width(), m_size.height(), style);

    ATOM classId = createWindowClass(createWindowClassName(this).c_str(), wndProc);
    if (!classId) {
        GENESIS_LOG_THROW_ERROR("RegisterClassEx failed.\nError code: 0x{:08X}", GetLastError());
    }

    m_handle = CreateWindowEx(
        0,
        MAKEINTATOM(classId),
        title.c_str(),
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );
    if (!m_handle) {
        GENESIS_LOG_THROW_ERROR("CreateWindowEx failed.\nError code: 0x{:08X}", GetLastError());
    }

    SetWindowLongPtr(static_cast<HWND>(m_handle), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
}

Win32Window::~Win32Window()
{
    DestroyWindow(static_cast<HWND>(m_handle));
}

void Win32Window::centerOnScreen()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    setPosition((screenWidth - m_size.width()) / 2, (screenHeight - m_size.height()) / 2);
}

void Win32Window::resize(uint32 width, uint32 height)
{
    if (m_size.width() == width && m_size.height() == height) {
        return;
    }

    RECT wndRect = createWindowRect(width, height, getWindowStyle(m_style));
    SetWindowPos(
        static_cast<HWND>(m_handle),
        HWND_TOP,
        0,
        0,
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top,
        SWP_NOMOVE
    );
}

void Win32Window::setPosition(uint32 x, uint32 y)
{
    SetWindowPos(
        static_cast<HWND>(m_handle),
        nullptr,
        x,
        y,
        0,
        0,
        SWP_NOSIZE | SWP_NOZORDER
    );
}

void Win32Window::setStyle(WindowStyle style)
{
    HWND hwnd = static_cast<HWND>(m_handle);

    SetWindowLongPtr(hwnd, GWL_STYLE, getWindowStyle(style));
    ShowWindow(hwnd, SW_SHOW);

    RECT wndRect = createWindowRect(m_size.width(), m_size.height(), getWindowStyle(style));
    SetWindowPos(
        hwnd,
        nullptr,
        0, 
        0, 
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top,
        SWP_NOMOVE | SWP_NOZORDER
    );

    m_style = style;
}

LRESULT CALLBACK Win32Window::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (!wnd) {
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    switch (msg) {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_SIZE: 
    {
        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);

        wnd->m_size = Rect{static_cast<int32>(width), static_cast<int32>(height)};
        if (wnd->m_onResize) {
            wnd->m_onResize(width, height);
        }

        return 0;
    }
    case WM_ACTIVATE:
    {
        WORD state = LOWORD(wparam);
        bool isFocused = (state != WA_INACTIVE);

        wnd->m_hasFocus = isFocused;
        if (wnd->m_onFocusChanged) {
            wnd->m_onFocusChanged(isFocused);
        }

        return 0;
    }
    case WM_SETCURSOR:
    {
        WORD hitTest = LOWORD(lparam);

        if (hitTest == HTCLIENT) {
            SetCursor(LoadCursor(nullptr, IDC_ARROW));
            return TRUE;
        }

        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

/* STATIC FUNCTION DEFINITIONS */

RECT createWindowRect(UINT width, UINT height, DWORD style)
{
    RECT wndRect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    AdjustWindowRect(&wndRect, style, false);

    return wndRect;
}

ATOM createWindowClass(LPCTSTR className, WNDPROC windowProc)
{
    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = 0;
    wndClass.lpfnWndProc = windowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = nullptr;
    wndClass.hIcon = nullptr;
    wndClass.hbrBackground = nullptr;
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = className;
    wndClass.hIconSm = nullptr;

    return RegisterClassEx(&wndClass);
}

DWORD getWindowStyle(WindowStyle style)
{
    switch (style) {
    case WindowStyle::Windowed:
        return WS_OVERLAPPEDWINDOW;
    case WindowStyle::Borderless:
        return WS_OVERLAPPED;
    default:
        return WS_OVERLAPPEDWINDOW;
    }
}

wstring createWindowClassName(void* instance)
{
    return wstring(BASE_WINDOW_CLASS_NAME) + L"_" + to_wstring(reinterpret_cast<uintptr_t>(instance));
}