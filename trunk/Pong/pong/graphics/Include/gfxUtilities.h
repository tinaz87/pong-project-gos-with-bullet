#pragma once
#include "dataTypes.h"
#include "d3d9.h"
#include <d3dx9.h>

class GfxRectangle
{
public:
	GfxRectangle();
	GfxRectangle(const uint32 x, const uint32 y, const uint32 width, const uint32 height);
	GfxRectangle(const RECT& rect);

	const RECT& getRectangle() const;
	RECT& editRectangle();

private:
	RECT m_rectangle;
};

class GfxColor
{
public:
	GfxColor();
	GfxColor(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha = 255);
	GfxColor(const D3DCOLOR& color);

	const D3DCOLOR& getColor() const;
	D3DCOLOR& editColor();

private:
	D3DCOLOR m_color;
};