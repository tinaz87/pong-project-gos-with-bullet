#include "physicsBody.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "glog/logging.h"
#include "GameObjectSystem.h"
#include "position.h"

const StringHash PhysicsBody::PHY_BODY_ID= StringHash("phyBody");

PhysicsBody::PhysicsBody(const ObjectId& objectId, 
						 real mass,
						 btCollisionShape* collisionShape,
						 const vector3& inertia, 
						 vector3 gfxOffset)
	:ObjectProperty(PHY_BODY_ID, objectId)
	,m_body(NULL),startVelocity(0,0,0),m_maxSpeed(100.0f)
{
	DLOG_ASSERT(mass >= 0.f);
	DLOG_ASSERT(collisionShape != NULL);

	Position* position= NULL;
	ObjectProperty* objectProperty= GameObjectSystem::GetSingleton().editProperty(Position::POSITION_ID, objectId);
	if(objectProperty != NULL)
	{
		position= static_cast<Position*>(objectProperty);
	}
	DLOG_ASSERT(position != NULL);

	btVector3 btInertia(mass * inertia.x, mass * inertia.y, mass * inertia.z);

	if ((mass > 0) && (Vector3LenghtSq(inertia) == 0))
		collisionShape->calculateLocalInertia(mass, btInertia);

	btMotionState* motionState= position->editMotionState();
	btRigidBody::btRigidBodyConstructionInfo constructorParams(mass, motionState, collisionShape, btInertia);
	constructorParams.m_linearDamping = 0.f;
	constructorParams.m_angularDamping = 0.0f;
	constructorParams.m_friction= 0.4f;
	constructorParams.m_restitution= 0.95f;

	//perform an aligned allocation required by SIMD instructions in btSequentialInputSolver
	m_body= MV_NEW_ALIGNED(16) btRigidBody(constructorParams);
	btTransform cmTransform;
	position->getMotionState()->getWorldTransform(cmTransform);
	m_body->setCenterOfMassTransform(cmTransform);
	if(mass > 0)
	{
		m_body->setCcdMotionThreshold(150.f);
		m_body->setCcdSweptSphereRadius(0.2f);
	}

	//StringHash a;

	
	m_body->btCollisionObject::setUserPointer(const_cast<void*>(static_cast< const void*>(&(ObjectProperty::getObjectId()))));

	reset();
}

PhysicsBody::~PhysicsBody()
{
	MV_DELETE(m_body);
}

void PhysicsBody::reset()
{
	btVector3 zero(btScalar(0.0), btScalar(0.0), btScalar(0.0));
	m_body->setLinearVelocity(zero);
	m_body->setAngularVelocity(zero);
	m_body->setAngularFactor(1);
	m_body->clearForces();
	m_body->setActivationState(ISLAND_SLEEPING);
}

void PhysicsBody::setMaximumSpeed(const float ispeed){

	m_maxSpeed = ispeed;
}

const float  PhysicsBody::getMaximumSpeed() const{

	return m_maxSpeed;
}

const btRigidBody* PhysicsBody::getBody() const
{
	return m_body;
}

btRigidBody* PhysicsBody::editBody()
{
	return m_body;
}

btCollisionShape* PhysicsBody::editCollisionShape()
{
	return m_body->getCollisionShape();
}

void PhysicsBody::setKinematic()
{
	m_body->setCollisionFlags( m_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); 
	m_body->setActivationState(DISABLE_DEACTIVATION);
}

void PhysicsBody::setVelocity(const vector3& velocity)
{
	btVector3 vel = btVector3(velocity.x, velocity.y, 0.f);
	m_body->setLinearVelocity(vel);

	real length= vel.length();
	if(length > 0.001f)
	{
		m_body->activate(true);
	}
}


void PhysicsBody::setSpeed(real speed)
{
	const btVector3& velocity= m_body->getLinearVelocity();
	real length= velocity.length();
	if(length > 0.001f)
	{
		m_body->setLinearVelocity(velocity/length*speed);
	}
	else
	{
		m_body->setLinearVelocity(btVector3(speed, 0.f, 0.f));
	}
}


const real PhysicsBody::getSpeed()const{

	const btVector3& velocity= m_body->getLinearVelocity();
	return velocity.length();

}

void PhysicsBody::setStartVelocity(const vector3& velocity){

	startVelocity = velocity;

}

const vector3& PhysicsBody::getStartVelocity()const{

	return startVelocity;

}