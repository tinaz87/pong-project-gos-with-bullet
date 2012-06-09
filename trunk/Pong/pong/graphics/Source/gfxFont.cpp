#include "gfxFont.h"

const StringHash GfxFont::GFX_FONT_ID = StringHash("gfxFont");

GfxFont::GfxFont(	const ObjectId& objectId, 
					const std::string& fontName, 
					const uint8 fontSize,
					const std::string& text,
					const GfxColor& fontColor,
					const GfxRectangle& destinationRectangle,
					bool isActive)
	:ObjectProperty(GFX_FONT_ID, objectId)
	,m_fontName(fontName)
	,m_fontSize(fontSize)
	,m_text(text)
	,m_fontColor(fontColor)
	,m_destinationRectangle(destinationRectangle)
	,m_isActive(isActive)
{
}

GfxFont::~GfxFont()
{
	
}

const std::string& GfxFont::getFontName() const
{
	return m_fontName;
}

const uint8& GfxFont::getFontSize() const
{
	return m_fontSize;
}

void GfxFont::setText(const std::string& text)
{
	m_text = text;
}

const std::string& GfxFont::getText() const
{
	return m_text;
}

void GfxFont::setFontColor(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha)
{
	m_fontColor = GfxColor(red, green, blue, alpha);
}

void GfxFont::setFontColor(const GfxColor& fontColor)
{
	m_fontColor = fontColor;
}

const GfxColor& GfxFont::getFontColor() const
{
	return m_fontColor;
}

void GfxFont::setRectangle(const GfxRectangle& rectangle)
{
	m_destinationRectangle = rectangle;
}

void GfxFont::setRectangle(uint32 x, uint32 y, uint32 width, uint32 height)
{
	m_destinationRectangle = GfxRectangle(x, y, width, height);
}

const GfxRectangle& GfxFont::getTextRectangle() const
{
	return m_destinationRectangle;
}

void GfxFont::setActive(bool active)
{
	m_isActive = active;
}

bool GfxFont::isActive() const
{
	return m_isActive;
}