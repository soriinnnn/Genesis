#include <math/Rect.h>

using namespace genesis;

// --------------------------------------------------------------------------------

Rect::Rect()
{
	this->left = 0;
	this->top = 0;
	this->right = 0;
	this->bottom = 0;
}

Rect::Rect(int width, int height)
{
	this->left = 0;
	this->top = 0;
	this->right = width;
	this->bottom = height;
}

Rect::Rect(int left, int top, int right, int bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

int Rect::width() const noexcept {
	return this->right - this->left;
}

int Rect::height() const noexcept {
	return this->bottom - this->top;
}