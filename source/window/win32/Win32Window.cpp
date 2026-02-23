#include <window/win32/Win32Window.h>

#define BASE_WINDOW_CLASS_NAME L"GenesisWindow"
#define DEFAULT_WINDOW_STYLE (WS_OVERLAPPEDWINDOW | WS_SIZEBOX)

using namespace genesis;
using namespace std;

static RECT createWindowRect(UINT width, UINT height);
static ATOM createWindowClass(LPCTSTR className, WNDPROC windowProc);
static wstring createWindowClassName(void* instance);

Win32Window::Win32Window(const WindowDesc& desc): Window(desc)
{
    ATOM classId = createWindowClass(createWindowClassName(this).c_str(), wndProc);
    if (!classId) {
        GENESIS_LOG_THROW_ERROR("RegisterClassEx failed.");
    }
    RECT wndRect = createWindowRect(m_size.width(), m_size.height());

    m_handle = CreateWindowEx(
        0,
        MAKEINTATOM(classId),
        desc.title,
        DEFAULT_WINDOW_STYLE,
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
        GENESIS_LOG_THROW_ERROR("CreateWindowEx failed.");
    }

    SetWindowLongPtr(static_cast<HWND>(m_handle), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
}

Win32Window::~Win32Window()
{
    DestroyWindow(static_cast<HWND>(m_handle));
}

void Win32Window::resize(uint32 width, uint32 height)
{
    if (m_size.width() == width && m_size.height() == height) {
        return;
    }

    RECT wndRect = createWindowRect(width, height);
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

LRESULT CALLBACK Win32Window::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Win32Window* wnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg) {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    case WM_SIZE: 
    {
        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);

        wnd->m_size = Rect{width, height};
        if (wnd->m_onResizeCallback) {
            wnd->m_onResizeCallback(width, height);
        }

        return 0;
    } 
    break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

/* STATIC FUNCTION DEFINITIONS */

static RECT createWindowRect(UINT width, UINT height)
{
    RECT wndRect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    AdjustWindowRect(&wndRect, DEFAULT_WINDOW_STYLE, false);

    return wndRect;
}

static ATOM createWindowClass(LPCTSTR className, WNDPROC windowProc)
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
};

static wstring createWindowClassName(void* instance)
{
    return wstring(BASE_WINDOW_CLASS_NAME) + L"_" + to_wstring(reinterpret_cast<uintptr_t>(instance));
}