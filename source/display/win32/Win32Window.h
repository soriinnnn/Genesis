#ifndef GENESIS_WIN32_WINDOW_H
#define GENESIS_WIN32_WINDOW_H
#include <display/Window.h>
#include <Windows.h>

GENESIS_NAMESPACE_BEGIN

class Win32Window final: public Window
{
public:
	explicit Win32Window(const WindowDesc& desc);
	~Win32Window() override;

	Rect getScreenSize() const override;

	void setPosition(const Point& position) override;
	void setSize(const Rect& size) override;
	void setStyle(WindowStyle style) override;
	void setTitle(const char* title) override;

	void show() override;
	void hide() override;
	void center() override;

private:
	void open();
	void close();

private:
	static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static LRESULT onCreate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT onDestroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT onClose(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT onMove(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT onSize(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT onShowWindow(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT onActivate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT onSetCursor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	HINSTANCE m_instance;
	HCURSOR m_cursor;
};

GENESIS_NAMESPACE_END

#endif