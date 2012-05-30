#include <cassert>
#include "HandleMgr.h"
#include "StringHash.h"
#include <d3dx9.h>
#include "Allocator.h" //std::vector std::map stdext::hash_map includes

// [ platform-specific surface handle type here ]
typedef LPDIRECT3DTEXTURE9 OsHandle;
typedef LPDIRECT3DDEVICE9  GfxDevice;

struct tagTexture  {  };
typedef Handle<tagTexture> HTexture;

class TextureMgr
{
public:
	// Lifetime
	TextureMgr(LPDIRECT3DDEVICE9 device)  
		{ m_dxDevice= device; }
	~TextureMgr();

	// Texture management
	HTexture getCreateTexture(const std::string& name);
	void	 deleteTexture(HTexture htex );

	OsHandle getTexture( HTexture htex, unsigned int mip = 0 ) const
		{  return ( m_Textures.dereference( htex )->getOsHandle( mip ) ); }

private:
	// Texture object data and db.
	struct Texture
	{
		typedef MvVector<OsHandle> HandleVec;

		StringHash   m_textureId;   // for reconstruction
		HandleVec    m_Handles;     // handles to mip surfaces

		Texture()
			:m_textureId("")
		{

		}

		OsHandle getOsHandle(uint16 mip) const
		{
			assert( mip < m_Handles.size() );
			return ( m_Handles[mip] );
		}

		bool load(LPDIRECT3DDEVICE9 device, const std::string& name);
		void unload();
	};
	typedef HandleMgr<Texture, HTexture> HTextureMgr;

	typedef MvMap<uint32, HTexture>					NameIndex;
	typedef std::pair<NameIndex::iterator, bool>	NameIndexInsertRc;

	// Private data.
	HTextureMgr			m_Textures;
	NameIndex			m_NameIndex;
	LPDIRECT3DDEVICE9	m_dxDevice;		
};