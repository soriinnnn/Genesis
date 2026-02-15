#include <math/Rect.h>

using namespace genesis;

// --------------------------------------------------------------------------------

Rect::Rect()
{
	m_left = 0;
	m_top = 0;
	m_right = 0;
	m_bottom = 0;
}

Rect::Rect(int width, int height)
{
	m_left = 0;
	m_top = 0;
	m_right = width;
	m_bottom = height;
}

Rect::Rect(int left, int top, int right, int bottom)
{
	m_left = left;
	m_top = top;
	m_right = right;
	m_bottom = bottom;
}

int Rect::width() const noexcept {
	return m_right - m_left;
}

int Rect::height() const noexcept {
	return m_bottom - m_top;
}