#include "Ball.h"
#include "gfxGeometry.h"
#include "gfxTexture.h"
#include "position.h"
#include "GameObjectSystem.h"
#include "physicsBody.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"

Ball::Ball(const std::string& ballId, const vector3& posOffset)
{
	ObjectId ballObjId= ObjectId(ballId.c_str());
	//Create geometry and texture
	GfxGeometry* geometry= MV_NEW GfxGeometry(ballObjId);
	vector4 vertices[]={{-1.5f, 1.5f, 0.5f, 1.f}, 
						{1.5f, 1.5f, 0.5f, 1.f}, 
						{1.5f, -1.5f, 0.5f, 1.f}, 
						{-1.5f, -1.5f, 0.5f, 1.f}};
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

 	GfxTexture* texture= MV_NEW GfxTexture(ballObjId, "ball.dds");
 	GameObjectSystem::GetSingleton().addProperty(texture);

	//Create position
	Position* bumperPosition= MV_NEW Position(ballObjId);
	quaternion initRotation= QuaternionFromRotationMatrix(MAT_IDENTITY);
	matrix matPos= MatrixPosition(posOffset, initRotation);
	bumperPosition->setPosition(matPos);
	GameObjectSystem::GetSingleton().addProperty(bumperPosition);

	//Create physics shape
	btSphereShape* sphereShape= MV_NEW btSphereShape(1.5f);
	sphereShape->setMargin(0.3f);
	PhysicsBody* ballBody= MV_NEW PhysicsBody(ballObjId, 10.f, sphereShape);
	ballBody->setVelocity(vector3(100.f,0.f,0.f));

	//Add the object
	GameObjectSystem::GetSingleton().addProperty(ballBody);
}

Ball::~Ball()
{

}