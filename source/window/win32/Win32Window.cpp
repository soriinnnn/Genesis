#include <window/Window.h>
#include <windows.h>

#define BASE_WINDOW_CLASS_NAME L"GenesisWindow"
#define DEFAULT_WINDOW_STYLE (WS_OVERLAPPEDWINDOW | WS_SIZEBOX)

using namespace genesis;
using namespace std;

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static RECT createWindowRect(UINT width, UINT height);
static ATOM createWindowClass(LPCTSTR className, WNDPROC windowProc);
static wstring createWindowClassName(void* instance);

// --------------------------------------------------------------------------------

Window::Window(const WindowDesc& desc): Base(desc.base), m_size(desc.size)
{
    RECT wndRect = createWindowRect(m_size.width(), m_size.height());
    ATOM classId = createWindowClass(createWindowClassName(this).c_str(), windowProc);
    if (!classId) {
        GENESIS_LOG_THROW_ERROR("RegisterClassEx failed.");
    }

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

Window::~Window() 
{
    DestroyWindow(static_cast<HWND>(m_handle));
}

void Window::resize(uint32 width, uint32 height)
{
    if (m_size.width() == width && m_size.height() == height) {
        return;
    }
    m_size = Rect{width, height};

    RECT wndRect = createWindowRect(width, height);
    SetWindowPos(
        static_cast<HWND>(m_handle),
        HWND_TOPMOST,
        0,
        0, 
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top,
        SWP_NOMOVE
    );
    onResize();
}

// --------------------------------------------------------------------------------

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg) {
    case WM_SIZE: {
        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);
        wnd->resize(width, height); 
    } break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

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