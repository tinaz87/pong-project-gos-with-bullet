#pragma once
#include "LinearMath/btIDebugDraw.h"
#include "graphicsDebugger.h"

class btCollisionWorld;

class PhysicsGfxDebug: public IGfxDebug
{
public:
	PhysicsGfxDebug(btCollisionWorld* world);
	virtual ~PhysicsGfxDebug() {}

	//IGfxDebug interface
	virtual void drawDebug(LPDIRECT3DDEVICE9 device);
	void addLine(const VERTEXCOLOR& from, const VERTEXCOLOR& to);

private:
	struct Line
	{
		VERTEXCOLOR vertex[2];
	};

	LPDIRECT3DDEVICE9 m_dxDevice;
	btCollisionWorld* m_world;
	MvVector<Line>	  m_lines;
};

class BulletDebugger: public btIDebugDraw
{
public:
	BulletDebugger(btCollisionWorld* world, GraphicsDebugger* graphicsDebugger);
	~BulletDebugger();

	//btIDebugDraw interface
	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color){}
	virtual void	reportErrorWarning(const char* warningString) {}
	virtual void	draw3dText(const btVector3& location, const char* textString) {}
	virtual void	setDebugMode(int debugMode) 
	{
		m_debugMode= debugMode;
	}

	virtual int		getDebugMode() const 
	{
		return m_debugMode;
	}

private:
	int				 m_debugMode;
	PhysicsGfxDebug* m_gfxDebug;
};