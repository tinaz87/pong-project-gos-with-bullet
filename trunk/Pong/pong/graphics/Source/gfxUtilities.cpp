#include "gfxUtilities.h"

GfxRectangle::GfxRectangle()
{

}

GfxRectangle::GfxRectangle(const uint32 x, const uint32 y, const uint32 width, const uint32 height)
{
	m_rectangle.left = x;
	m_rectangle.top = y;
	m_rectangle.right = m_rectangle.left + width;
	m_rectangle.bottom = m_rectangle.top + height;
}

GfxRectangle::GfxRectangle(const RECT& rect)
	:m_rectangle(rect)
{

}

const RECT& GfxRectangle::getRectangle() const
{
	return m_rectangle;
}

RECT& GfxRectangle::editRectangle()
{
	return m_rectangle;
}

GfxColor::GfxColor()
	:m_color(D3DCOLOR_ARGB(255, 255, 255, 255))
{

}

GfxColor::GfxColor(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha)
	:m_color(D3DCOLOR_ARGB(255, red, green, blue))
{

}

GfxColor::GfxColor(const D3DCOLOR& color)
	:m_color(color)
{

}

const D3DCOLOR& GfxColor::getColor() const
{
	return m_color;
}

D3DCOLOR& GfxColor::editColor()
{
	return m_color;
}