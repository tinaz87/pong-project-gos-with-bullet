#pragma once
#include "ObjectProperty.h"
#include "gfxUtilities.h"

class GfxFont: public ObjectProperty
{
public:
	static const StringHash GFX_FONT_ID;

	GfxFont(const ObjectId& objectId, 
			const std::string& fontName = "Arial", 
			const uint8 fontSize = 20,
			const std::string& text = "",
			const GfxColor& fontColor = GfxColor(255, 255, 255, 255),
			const GfxRectangle& destinationRectangle = GfxRectangle(0, 0, 0, 0),
			bool isActive = false);
	~GfxFont();
	
	const std::string& getFontName() const;
	const uint8& getFontSize() const;

	void setText(const std::string& text = "");
	const std::string& getText() const;

	void setFontColor(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha = 255);
	void setFontColor(const GfxColor& fontColor = GfxColor(255, 255, 255, 255));
	const GfxColor& getFontColor() const;

	void setRectangle(const GfxRectangle& rectangle);
	void setRectangle(uint32 x, uint32 y, uint32 width, uint32 height);
	const GfxRectangle& getTextRectangle() const;

	void setActive(bool active);
	bool isActive() const;

private:
	std::string		m_fontName;
	uint8			m_fontSize;
	
	std::string		m_text;

	GfxColor		m_fontColor;
	GfxRectangle	m_destinationRectangle;

	bool			m_isActive;
};