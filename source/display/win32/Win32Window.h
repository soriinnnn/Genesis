#ifndef GENESIS_WIN32_WINDOW_H
#define GENESIS_WIN32_WINDOW_H
#include <display/Window.h>
#include <windows.h>

namespace genesis
{
	class Win32Window final: public Window
	{
	public:
		explicit Win32Window(const WindowDesc& desc);
		~Win32Window() override;

		void center() override;
		void resize(uint32 width, uint32 height) override;
		void setPosition(uint32 x, uint32 y) override;
		void setStyle(WindowStyle style) override;

	private:
		static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		uint32 wndStyle;
	};
}

#endif