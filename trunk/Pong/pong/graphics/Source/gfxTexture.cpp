#include "gfxTexture.h"

const StringHash GfxTexture::GFX_TEXTURE_ID= StringHash("gfxTex");

GfxTexture::GfxTexture(const ObjectId& objectId, const std::string& textureName)
	:ObjectProperty(GFX_TEXTURE_ID, objectId)
	,m_textureName(textureName)
{
}
	
GfxTexture::~GfxTexture()
{
}

const std::string& GfxTexture::getTextureName() const
{
	return m_textureName;
}