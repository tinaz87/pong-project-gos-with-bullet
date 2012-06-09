#include "fontManager.h"
#include <glog/logging.h>
#include <WinError.h>
#include <sstream>
FontMgr::~FontMgr()
{
	// Release all our fonts.
	NameIndex::iterator i, begin = m_NameIndex.begin(), end = m_NameIndex.end();
	for (i = begin; i != end; ++i)
	{
		Font* font = m_Fonts.dereference(i->second);
		if (font != NULL)
			font->unload();
	}
}

HFont FontMgr::getCreateFont(const std::string& name, const uint8 size)
{
	std::stringstream s_stream;
	s_stream<<name<<(static_cast<char>(size));
	
	std::string ss = s_stream.str();
	// Insert/find.
	std::pair<NameIndex::iterator, bool> rc = m_NameIndex.insert(std::make_pair(StringHash((const char*)s_stream.str().c_str()).GetHash(), HFont()));
	if (rc.second)
	{
		// This is a new insertion.
		Font* font = m_Fonts.acquire(rc.first->second);
		if (!font->load(m_dxDevice, name, size))
		{
			// Load failed. Release handle.
			deleteFont(rc.first->second);
			// Insert a null handle.
			rc = m_NameIndex.insert(std::make_pair(StringHash((const char*)s_stream.str().c_str()).GetHash(), HFont()));
		}
	}
	return (rc.first->second);
}

void FontMgr::deleteFont(HFont hfont)
{
	Font* font = m_Fonts.dereference(hfont);
	if (font != 0)
	{
		// Delete from index.
		m_NameIndex.erase(m_NameIndex.find(font->m_fontId.GetHash()));

		// Delete from db.
		font->unload();
		m_Fonts.release(hfont);
	}
}

bool FontMgr::Font::load(LPDIRECT3DDEVICE9 device, const std::string& name, const uint8 size)
{
	std::stringstream s_stream;
	s_stream<<name<<(static_cast<char>(size));
	m_fontId = StringHash((const char*)s_stream.str().c_str());
	OsHandle pFont = NULL;
	if ( FAILED(D3DXCreateFontA(device, size, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &pFont)) )
	{
		DLOG(WARNING) << "error finding resource" << s_stream.str().c_str();
	}
	m_Handle = pFont;
	return pFont != NULL;
}

void FontMgr::Font::unload(void)
{
	m_fontId = StringHash("");
	if (m_Handle != NULL)
	{
		m_Handle->Release();
	}
}