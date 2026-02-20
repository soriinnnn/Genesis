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

Rect::Rect(uint32 width, uint32 height)
{
	this->left = 0;
	this->top = 0;
	this->right = width;
	this->bottom = height;
}

Rect::Rect(uint32 left, uint32 top, uint32 right, uint32 bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

uint32 Rect::width() const noexcept {
	return this->right - this->left;
}

uint32 Rect::height() const noexcept {
	return this->bottom - this->top;
}