#include "graphicsDebugger.h"

const StringHash IGfxDebug::GFX_DEBUG_ELEM_ID= StringHash("gfxDebugElem");
const StringHash GraphicsDebugger::GRAPHICS_DEBUGGER_ID= StringHash("gfxDebug");
const StringHash GraphicsDebugger::GRAPHICS_DEBUGGER_OBJ_ID= StringHash("drawDebug");

GraphicsDebugger::GraphicsDebugger()
	:ObjectProperty(GRAPHICS_DEBUGGER_ID, GRAPHICS_DEBUGGER_OBJ_ID)
{

}

GraphicsDebugger::~GraphicsDebugger()
{
	while(!m_gfxDebugArray.empty())
	{
		IGfxDebug* gfxDebug= m_gfxDebugArray.back();
		MV_DELETE(gfxDebug);
		m_gfxDebugArray.pop_back();
	}

}

void GraphicsDebugger::addGfxDebug(IGfxDebug* gfxDebug)
{
	assert(gfxDebug!= NULL);
	m_gfxDebugArray.push_back(gfxDebug);
}

void GraphicsDebugger::drawDebug(LPDIRECT3DDEVICE9 device) const
{
	for(uint32 i= 0; i < m_gfxDebugArray.size(); ++i)
	{
		m_gfxDebugArray[i]->drawDebug(device);
	}
}