#include "background.h"
#include "GameObjectSystem.h"
#include "gfxGeometry.h"
#include "gfxTexture.h"
#include "CustomMemory.h"
#include "physicsBody.h"
#include "LinearMath/btVector3.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "Position.h"

Background::Background(const char* backgroundFilename)
	:m_backgroundFilename(backgroundFilename)
{
	GfxGeometry* geometry= MV_NEW GfxGeometry(ObjectId("background"));
	vector4 vertices[]={{-100.0f, 100.0f, 0.f, 1.f}, 
						{100.0f, 100.0f, 0.f, 1.f}, 
						{100.0f, -100.0f, 0.f, 1.f}, 
						{-100.0f, -100.0f, 0.f, 1.f}};
	vector2 textCoord[]={vector2(0.0f, 1.0f), vector2(1.0f, 1.0f), vector2(1.0f, 0.0f), vector2(0.0f, 0.0f)};
	// 3--------------2
	// |              |
	// |              |
	// 0--------------1
	uint32 indices[]= {2, 1, 0,
					   3, 2, 0};
	geometry->setTriangles(indices, 6);
	geometry->setVertices(vertices, 4);
	geometry->setTextureCoord(textCoord, 4);
	GameObjectSystem::GetSingleton().addProperty(geometry);

 	GfxTexture* texture= MV_NEW GfxTexture(ObjectId("background"), "background.jpg");
 	GameObjectSystem::GetSingleton().addProperty(texture);

	//create 4 physics walls
	btBoxShape* wallShape= MV_NEW btBoxShape(btVector3(10.f, 100.f, 10.f));


	vector3 wallRightTrx(107.5f, 0.f, 0.f);
	quaternion wallLateralRot= QuaternionFromRotationMatrix(MAT_IDENTITY);
	Position* wallRightPosition= MV_NEW Position("wall1");
	wallRightPosition->setPosition(MatrixPosition(wallRightTrx, wallLateralRot));
	GameObjectSystem::GetSingleton().addProperty(wallRightPosition);
	PhysicsBody* wallRight= MV_NEW PhysicsBody("wall1", 0.f, wallShape);
	
	vector3 wallLeftTrx(-107.5f, 0.f, 0.f);
	Position* wallLeftPosition= MV_NEW Position("wall2");
	wallLeftPosition->setPosition(MatrixPosition(wallLeftTrx, wallLateralRot));
	GameObjectSystem::GetSingleton().addProperty(wallLeftPosition);
	PhysicsBody* wallLeft= MV_NEW PhysicsBody("wall2", 0.f, wallShape);
	
	quaternion wallUpDownRot= InitQuaternion(0.f, 0.f, sinf(PI/4.f), cosf(PI/4.f));
	vector3 wallUpTrx(0.f, -107.5f, 0.f);
	Position* wallUpPosition= MV_NEW Position("wall3");
	wallUpPosition->setPosition(MatrixPosition(wallUpTrx, wallUpDownRot));
	GameObjectSystem::GetSingleton().addProperty(wallUpPosition);
	PhysicsBody* wallUp= MV_NEW PhysicsBody("wall3", 0.f, wallShape);
	
	vector3 wallDownTrx(0.f, 107.5f, 0.f);
	Position* wallDownPosition= MV_NEW Position("wall4");
	wallDownPosition->setPosition(MatrixPosition(wallDownTrx, wallUpDownRot));
	GameObjectSystem::GetSingleton().addProperty(wallDownPosition);
	PhysicsBody* wallDown= MV_NEW PhysicsBody("wall4", 0.f, wallShape);
	
	GameObjectSystem::GetSingleton().addProperty(wallRight);
	GameObjectSystem::GetSingleton().addProperty(wallLeft);
	GameObjectSystem::GetSingleton().addProperty(wallUp);
	GameObjectSystem::GetSingleton().addProperty(wallDown);
}

Background::~Background()
{

}