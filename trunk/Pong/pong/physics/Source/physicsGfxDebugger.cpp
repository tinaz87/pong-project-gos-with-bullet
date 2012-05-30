#include "physicsGfxDebugger.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "d3dx9.h"

//Utility class
PhysicsGfxDebug::PhysicsGfxDebug(btCollisionWorld* world)
	:m_world(world)
{
	assert(m_world);
}

void PhysicsGfxDebug::drawDebug(LPDIRECT3DDEVICE9 device)
{
	m_dxDevice= device;
	m_lines.clear();
	m_world->debugDrawWorld();

	m_dxDevice->SetTexture( 0, NULL);
	m_dxDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	matrix matWorld= MAT_IDENTITY;
	m_dxDevice->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)(&matWorld));
	for(uint32 i= 0; i < m_lines.size(); ++i)
	{
	//	m_dxDevice->SetStreamSource( 0, NULL, 0, sizeof( VERTEXCOLOR ) );
		m_dxDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &m_lines[i], sizeof(VERTEXCOLOR)); // poco efficiente ma è a scopo di debug
	}
}


void PhysicsGfxDebug::addLine(const VERTEXCOLOR& from, const VERTEXCOLOR& to)
{
	Line line;
	line.vertex[0]= from;
	line.vertex[1]= to;
	m_lines.push_back(line);
}

//DRAWER
BulletDebugger::BulletDebugger(btCollisionWorld* world, GraphicsDebugger* graphicsDebugger)
{
 	m_gfxDebug= MV_NEW PhysicsGfxDebug(world);
 	graphicsDebugger->addGfxDebug(m_gfxDebug);
 	world->setDebugDrawer(this);
 	setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

BulletDebugger::~BulletDebugger()
{
	//m_gfxDebug deleted in GraphicsDebugger
}

void	BulletDebugger::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	VERTEXCOLOR vertex[2];
	
	vertex[0].x= from.x();
	vertex[0].y= from.y();
	vertex[0].z= from.z();
	vertex[0].color= D3DCOLOR_XRGB(uint32(color.x()*255.f), uint32(color.y()*255.f), uint32(color.z()*255.f));
		
	vertex[1].x= to.x();
	vertex[1].y= to.y();
	vertex[1].z= to.z();
	vertex[1].color= D3DCOLOR_XRGB(uint32(color.x()*255.f), uint32(color.y()*255.f), uint32(color.z()*255.f));
	m_gfxDebug->addLine(vertex[0], vertex[1]);
}