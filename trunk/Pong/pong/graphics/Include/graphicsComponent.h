#pragma once
#include "d3d9.h"
#include "Component.h"
#include "textureManager.h"
#include "Allocator.h" //std::vector std::map stdext::hash_map includes
#include "FontManager.h"


class TextureMgr;
class Position;
class GraphicsDebugger;
class GfxFont;

class GraphicsComponent: public Component
{
public:
	static const StringHash GRAPHICS_COMPONENT_ID;
	GraphicsComponent(HWND hWnd);
	~GraphicsComponent();
	virtual void update(real frametime, real timestep);
	LPDIRECT3DDEVICE9 getDevice();

private:
	void setupCamera();
	void setPosition(const Position*);


	// Directx Global Variables
	LPDIRECT3D9              m_pD3D;					// Used to create the D3DDevice
	LPDIRECT3DDEVICE9		 m_pd3dDevice;				// Our rendering device
 	MvVector<LPDIRECT3DVERTEXBUFFER9> m_pVB;			// Buffer to hold Vertices
	MvVector<LPDIRECT3DINDEXBUFFER9>  m_pIB;			// Buffer to hold Indices
	MvVector<HTexture>				 m_pTextureB;	// Buffer to hold Textures
	MvVector<uint32>		 m_verticesCount;
	MvVector<uint32>		 m_primitivesCount;
	MvVector<const Position*>m_position;
	TextureMgr*				 m_textureManager;
	
	const GraphicsDebugger*	 m_graphicsDebugger;	


	MvVector<HFont>		m_Hfont;
	MvVector<const GfxFont*> m_font;

	FontMgr*	m_fontManager;
};