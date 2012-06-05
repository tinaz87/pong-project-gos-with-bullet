#pragma once
#include "Component.h"
#include "Publisher.h"
#include "collisionObserver.h"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

class  btDefaultCollisionConfiguration;
class  btCollisionDispatcher;
struct btDbvtBroadphase;
class  btSequentialImpulseConstraintSolver;
class  btDiscreteDynamicsWorld;
class  btCollisionShape;
typedef MvHashMap<uint32, btCollisionShape*> CollisionShapeMap;
class BulletDebugger;
class btRigidBody;


class PhysicsComponent: public Component
{
public:
	static const StringHash PHYSICS_COMPONENT_ID;
	PhysicsComponent(uint32 maxCollisionAlgorithmPoolSize, uint32 maxPersistentManifoldPoolSize);
	~PhysicsComponent();
	virtual void update(real frametime, real timestep);
	static  void internalTickCallback(btDynamicsWorld* world, real timeStep);
	void	internalTickHandler(btDynamicsWorld* world, real timeStep);

	real getGravityAcceleration() const;
	void setGravityAcceleration(real gravityAcceleration);

	void fireCollisionEvent(const ObjectId& idObjA,const ObjectId& idObjB);

	Publisher<CollisionObserver>& getCollisionPublisher();

	btDiscreteDynamicsWorld* getWorld(){

		return m_world;
	}

	void reset();

private:
	void								AddGfxDebug();
	void								AddRegisteredPhysicsObjects();
	real								m_gravityAcceleration;
	real								m_accumulator;

	btDefaultCollisionConfiguration*	m_collisionConfiguration;
	btCollisionDispatcher*				m_collisionDispatcher;
	btDbvtBroadphase*					m_broadphaseStrategy;
	btSequentialImpulseConstraintSolver*m_constraintSolver;
	btDiscreteDynamicsWorld*			m_world;
	CollisionShapeMap					m_collisionShapeMap;
	BulletDebugger*						m_gfxDebugger;
	static PhysicsComponent*			s_activePhysicsComponent;

	Publisher<CollisionObserver>	m_collisionPublisher;
};