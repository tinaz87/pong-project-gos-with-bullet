#pragma once
#include "ObjectProperty.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

class btRigidBody;
class btCollisionShape;

class PhysicsBody: public ObjectProperty
{
public:
	static const StringHash PHY_BODY_ID;
	PhysicsBody(const ObjectId& objectId, 
				real mass, 
				btCollisionShape* collisionShape, 	
				const vector3& inertia= vector3(0.f, 0.f, 0.f), 
				vector3 gfxOffset= vector3(0.f, 0.f, 0.f));
	~PhysicsBody();
	void			   reset();
	const btRigidBody* getBody() const;
	btRigidBody*       editBody();
	btCollisionShape*  editCollisionShape();
	void			   setKinematic();

	void			   setVelocity(const vector3& velocity);


	void			   setSpeed(real speed);
	const real		   getSpeed() const;


	const vector3& getStartVelocity()const;
	void setStartVelocity(const vector3& velocity);

	void setMaximumSpeed(const float ispeed);

	const float getMaximumSpeed()const;

private:
	btRigidBody*	m_body;
	float			m_maxSpeed;
	vector3			startVelocity;

};