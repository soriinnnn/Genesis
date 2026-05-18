#include <math/Rect.h>

using namespace genesis;

Rect::Rect()
{
	this->left = 0;
	this->top = 0;
	this->right = 0;
	this->bottom = 0;
}

Rect::Rect(int32 width, int32 height)
{
	this->left = 0;
	this->top = 0;
	this->right = width;
	this->bottom = height;
}

Rect::Rect(int32 left, int32 top, int32 right, int32 bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

int32 Rect::width() const noexcept {
	return this->right - this->left;
}

int32 Rect::height() const noexcept {
	return this->bottom - this->top;
}

bool Rect::operator==(const Rect& rhs) const noexcept
{
	return this->left == rhs.left && this->top == rhs.top && this->right == rhs.right && this->bottom == rhs.bottom;
}
