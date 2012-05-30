#pragma once
#include "ObjectProperty.h"
#include "d3d9.h"

class IGfxDebug
{
public:
	static const StringHash GFX_DEBUG_ELEM_ID;
	IGfxDebug() {}
	virtual ~IGfxDebug() {}
	virtual void drawDebug(LPDIRECT3DDEVICE9 device)= 0;
};

class GraphicsDebugger: public ObjectProperty
{
public:
	static const StringHash GRAPHICS_DEBUGGER_ID;
	static const StringHash GRAPHICS_DEBUGGER_OBJ_ID;
	GraphicsDebugger();
	~GraphicsDebugger();
	void addGfxDebug(IGfxDebug* gfxDebug);
	void drawDebug(LPDIRECT3DDEVICE9 device) const;
private:
	MvVector<IGfxDebug*> m_gfxDebugArray;
};