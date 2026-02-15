#include <window/Window.h>
#include <windows.h>
#include <string>

#define BASE_WINDOW_CLASS_NAME L"GenesisWindow"
#define DEFAULT_WINDOW_STYLE (WS_OVERLAPPEDWINDOW | WS_SIZEBOX)

using namespace genesis;
using namespace std;

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static ATOM createWindowClass(LPCTSTR className, WNDPROC windowProc);
static wstring createWindowClassName(void* instance);

// --------------------------------------------------------------------------------

Window::Window(const WindowDesc& desc): Base(desc.base), m_size(desc.size)
{
    ATOM classId = createWindowClass(createWindowClassName(this).c_str(), windowProc);
    if (!classId) {
        GENESIS_LOG_THROW_ERROR("RegisterClassEx failed.");
    }

    RECT windowRect = {0, 0, m_size.width(), m_size.height()};
    AdjustWindowRect(&windowRect, DEFAULT_WINDOW_STYLE, false);

    m_handle = CreateWindowEx(
        0,
        MAKEINTATOM(classId),
        desc.title,
        DEFAULT_WINDOW_STYLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );
    if (!m_handle) {
        GENESIS_LOG_THROW_ERROR("CreateWindowEx failed.");
    }

    ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
}

Window::~Window() 
{
    DestroyWindow(static_cast<HWND>(m_handle));
}

// --------------------------------------------------------------------------------

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

static ATOM createWindowClass(LPCTSTR className, WNDPROC windowProc)
{
    WNDCLASSEX windowClass = {};

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = 0;
    windowClass.lpfnWndProc = windowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = nullptr;
    windowClass.hIcon = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = className;
    windowClass.hIconSm = nullptr;

    return RegisterClassEx(&windowClass);
};

static wstring createWindowClassName(void* instance)
{
    return wstring(BASE_WINDOW_CLASS_NAME) + L"_" + to_wstring(reinterpret_cast<uintptr_t>(instance));
}