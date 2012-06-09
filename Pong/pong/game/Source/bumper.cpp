#include "bumper.h"
#include "gfxGeometry.h"
#include "gfxTexture.h"
#include "position.h"
#include "physicsBody.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "GameObjectSystem.h"
#include "SfxEffect.h"


Bumper::Bumper(const std::string& bumperId, const vector3& posOffset, bool setKinematic)
{
	ObjectId bumperObjId = ObjectId(bumperId.c_str()); //TODO: Schifo.... per adesso ok... Metti a posto
	//Create geometry and texture
	GfxGeometry* geometry= MV_NEW GfxGeometry(bumperObjId);
	vector4 vertices[]={{-3.0f, 15.0f, 0.5f, 1.f}, 
						{3.0f, 15.0f, 0.5f, 1.f}, 
						{3.0f, -15.0f, 0.5f, 1.f}, 
						{-3.0f, -15.0f, 0.5f, 1.f}};
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

 	GfxTexture* texture= MV_NEW GfxTexture(bumperObjId, "bumper.jpg");
 	GameObjectSystem::GetSingleton().addProperty(texture);

	// Create Sound

	SfxEffect* sound = MV_NEW SfxEffect(bumperObjId);
	sound->setSound("../Media/Wavs/Blip.wav");
	sound->setLoopActive(false);
	GameObjectSystem::GetSingleton().addProperty(sound);


	//Create position
	Position* bumperPosition= MV_NEW Position(bumperObjId);
	quaternion initRotation= QuaternionFromRotationMatrix(MAT_IDENTITY);
	matrix matPos= MatrixPosition(posOffset, initRotation);
	bumperPosition->setStartPosition(matPos);
	bumperPosition->setPosition(matPos);
	GameObjectSystem::GetSingleton().addProperty(bumperPosition);

	//Create physics shape
	btBoxShape* bumperShape= MV_NEW btBoxShape(btVector3(3.f, 15.f, 5.f));
	PhysicsBody* bumperBody= MV_NEW PhysicsBody(bumperObjId, 0.f, bumperShape);
	if(setKinematic)
	{
		bumperBody->setKinematic();
	}

	//Add the object
	GameObjectSystem::GetSingleton().addProperty(bumperBody);
}

Bumper::~Bumper()
{

}