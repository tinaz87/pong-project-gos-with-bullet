#pragma once
#include "ObjectProperty.h"

class GfxTexture: public ObjectProperty
{
public:
	static const StringHash GFX_TEXTURE_ID;
	GfxTexture(const ObjectId& objectId, const std::string& textureName);
	~GfxTexture();
	const std::string& getTextureName() const;
private:
	std::string	m_textureName;
};