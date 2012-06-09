#pragma once
#include "HandleMgr.h"
#include "StringHash.h"
#include <d3dx9.h>
#include "Allocator.h" //std::vector std::map stdext::hash_map includes



struct tagFont{ };
typedef Handle<tagFont> HFont;

class FontMgr
{
public:

	typedef LPD3DXFONT			OsHandle;
	typedef LPDIRECT3DDEVICE9	GfxDevice;

	// Lifetime
	FontMgr(LPDIRECT3DDEVICE9 device)
		{ m_dxDevice = device; }

	~FontMgr();

	// Font management.
	HFont getCreateFont(const std::string& name, const uint8 size);
	void deleteFont(HFont hfont);

	OsHandle getFont(HFont hfont) const
		{ return (m_Fonts.dereference(hfont)->getOsHandle()); }

private:
	struct Font
	{
		StringHash	m_fontId;
		OsHandle	m_Handle;

		Font()
			:m_fontId("")
			,m_Handle(NULL)
		{

		}

		OsHandle getOsHandle() const
		{
			return m_Handle;
		}

		bool load(LPDIRECT3DDEVICE9 device, const std::string& name, const uint8 size);
		void unload(void);
	};
	typedef HandleMgr<Font, HFont> HFontMgr;

	typedef MvMap<uint32, HFont>					NameIndex;
	typedef std::pair<NameIndex::iterator, bool>	NameIndexInsertRc;

	// Private data.
	HFontMgr			m_Fonts;
	NameIndex			m_NameIndex;
	LPDIRECT3DDEVICE9	m_dxDevice;
};