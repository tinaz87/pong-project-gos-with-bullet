#include "textureManager.h"
#include <glog/logging.h>
#include <WinError.h>

TextureMgr::~TextureMgr()
{
	// release all our remaining textures before we go
	NameIndex::iterator i, begin = m_NameIndex.begin(), end = m_NameIndex.end();
	for ( i = begin ; i != end ; ++i )
	{
		Texture* texture= m_Textures.dereference( i->second );
		if(texture != NULL)
			texture->unload();
	}
}

HTexture TextureMgr::getCreateTexture(const std::string& name)
{
	// insert/find
	std::pair<NameIndex::iterator, bool> rc= m_NameIndex.insert(std::make_pair(StringHash((const char*)name.c_str()).GetHash(), HTexture()));
	if (rc.second)
	{
		// this is a new insertion
		Texture* tex = m_Textures.acquire(rc.first->second);
		if ( !tex->load(m_dxDevice, name) )
		{
			//load failed...release handle and...
			deleteTexture(rc.first->second);
			//insert a null handle
			rc= m_NameIndex.insert(std::make_pair(StringHash((const char*)name.c_str()).GetHash(), HTexture()));
		}
	}
	return ( rc.first->second );
}

void TextureMgr::deleteTexture( HTexture htex )
{
	Texture* tex = m_Textures.dereference( htex );
	if ( tex != 0 )
	{
		// delete from index
		m_NameIndex.erase( m_NameIndex.find( tex->m_textureId.GetHash() ) );

		// delete from db
		tex->unload();
		m_Textures.release( htex );
	}
}

bool TextureMgr::Texture::load(LPDIRECT3DDEVICE9 dxDevice, const std::string& name )
{
	m_textureId = StringHash((const char*)name.c_str());
	OsHandle pTexture= NULL;
	if( FAILED( D3DXCreateTextureFromFileA( dxDevice, name.c_str(), &pTexture) ) )
	{
		DLOG(WARNING) << "error finding resource" << name.c_str();

	}
	m_Handles.push_back(pTexture);
	return pTexture!=NULL;
}

void TextureMgr::Texture::unload( void )
{
	m_textureId= StringHash("");
	while(!m_Handles.empty())
	{
		OsHandle texture= m_Handles.back();
		if(texture != NULL)
		{
			texture->Release();
		}
		m_Handles.pop_back();
	}
}
