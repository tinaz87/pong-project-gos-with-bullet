#include "physicsComponent.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include "GameObjectSystem.h"
#include "physicsBody.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "physicsGfxDebugger.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "glog/logging.h"
#include "LinearMath/btAlignedAllocator.h"
#include "position.h"

const StringHash PhysicsComponent::PHYSICS_COMPONENT_ID= StringHash("PhyCmp");
PhysicsComponent* PhysicsComponent::s_activePhysicsComponent= NULL;

PhysicsComponent::PhysicsComponent(uint32 maxCollisionAlgorithmPoolSize, uint32 maxPersistentManifoldPoolSize)
	:Component(PHYSICS_COMPONENT_ID, 1)
	,m_gravityAcceleration(0.f)
	,m_accumulator(0.f)
	,m_collisionConfiguration(NULL)
	,m_collisionDispatcher(NULL)
	,m_broadphaseStrategy(NULL)
	,m_constraintSolver(NULL)
	,m_world(NULL)
	,m_gfxDebugger(NULL)
{
	btAlignedAllocSetCustom(MvAlloc, MvFree);
	btAlignedAllocSetCustomAligned(MvMalloc, MvFree);

	btDefaultCollisionConstructionInfo constructionInfo;
	constructionInfo.m_defaultMaxCollisionAlgorithmPoolSize = maxCollisionAlgorithmPoolSize;
	constructionInfo.m_defaultMaxPersistentManifoldPoolSize = maxPersistentManifoldPoolSize;

	m_collisionConfiguration= MV_NEW btDefaultCollisionConfiguration(constructionInfo);
	m_collisionDispatcher = MV_NEW	btCollisionDispatcher(m_collisionConfiguration);
	m_broadphaseStrategy = MV_NEW btDbvtBroadphase();
	m_constraintSolver = MV_NEW btSequentialImpulseConstraintSolver();
	m_world= MV_NEW btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphaseStrategy, m_constraintSolver, m_collisionConfiguration);
	m_world->setInternalTickCallback(&PhysicsComponent::internalTickCallback);
	m_world->setGravity(btVector3(0.f, -getGravityAcceleration(), 0.f));
	AddGfxDebug();
	AddRegisteredPhysicsObjects();
}

void PhysicsComponent::AddGfxDebug()
{
	assert(m_world);
	GameObjectSystem& gameObjectSystem= GameObjectSystem::GetSingleton();
	ObjectProperty* objProp= gameObjectSystem.editProperty(GraphicsDebugger::GRAPHICS_DEBUGGER_ID, GraphicsDebugger::GRAPHICS_DEBUGGER_OBJ_ID);
	if(objProp != NULL)
	{
		GraphicsDebugger* graphicsDebugger= static_cast<GraphicsDebugger*>(objProp);
		m_gfxDebugger= MV_NEW BulletDebugger(m_world, graphicsDebugger);
	}
}

void PhysicsComponent::AddRegisteredPhysicsObjects()
{
	GameObjectSystem& gameObjectSystem= GameObjectSystem::GetSingleton();
	ObjectPropertyTable* physicsMap=  gameObjectSystem.editProperties(PhysicsBody::PHY_BODY_ID);
	if(physicsMap!=NULL)
	{
		for(ObjectPropertyTable::iterator opIt= physicsMap->begin(); opIt != physicsMap->end(); ++opIt)
		{
			PhysicsBody* physicsBody= static_cast<PhysicsBody*>(opIt->second);
			m_world->addRigidBody(physicsBody->editBody());
			if(physicsBody->getBody()->getInvMass()>0.f)
 			{
				btGeneric6DofConstraint* constraint= MV_NEW btGeneric6DofConstraint(*physicsBody->editBody(), btTransform::getIdentity(), false);
				constraint->setLinearLowerLimit(btVector3(-200.f, -200.f, 0.f));
				constraint->setLinearUpperLimit(btVector3(200.f, 200.f, 0.0001f));
				constraint->setAngularLowerLimit(btVector3(0.f,0.f,0.f));
				constraint->setAngularUpperLimit(btVector3(0.00001f,0.00001f,0.00001f));
				physicsBody->editBody()->activate();
				m_world->addConstraint(constraint);
 			}
			m_collisionShapeMap[uint32(physicsBody->editCollisionShape())]= physicsBody->editCollisionShape();
		}
	}
}

PhysicsComponent::~PhysicsComponent()
{
	while(m_world->getNumConstraints()>0)
	{
		btTypedConstraint* constraint= m_world->getConstraint(0);
		m_world->removeConstraint(constraint);
		MV_DELETE(constraint);
	}

	if(m_gfxDebugger!= NULL)
	{
		MV_DELETE(m_gfxDebugger);
	}
	
	MV_DELETE(m_world);
	MV_DELETE(m_constraintSolver);
	MV_DELETE(m_broadphaseStrategy);
	MV_DELETE(m_collisionDispatcher);
	MV_DELETE(m_collisionConfiguration);
	for(CollisionShapeMap::iterator csIt= m_collisionShapeMap.begin(); csIt != m_collisionShapeMap.end(); ++csIt)
	{
		btCollisionShape* collisionShape= csIt->second;
		MV_DELETE(collisionShape);
	}
	m_collisionShapeMap.clear();
	if(s_activePhysicsComponent== this)
	{
		s_activePhysicsComponent= NULL;
	}
}


void PhysicsComponent::reset(){

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();
	ObjectPropertyTable* opt = gameobject.editProperties(PhysicsBody::PHY_BODY_ID);

	if (opt != NULL)
	{
		
		for (ObjectPropertyTable::iterator it = opt->begin();it != opt->end();++it)
		{
			PhysicsBody* physicBody = static_cast<PhysicsBody*>(it->second);
			btRigidBody* rigidBody = physicBody->editBody();
			
			ObjectProperty* objectProperty = gameobject.editProperty(Position::POSITION_ID,physicBody->getObjectId());

			if (objectProperty != NULL)
			{
				Position* position = static_cast<Position*>(objectProperty);

				matrix s_position = position->getStartPosition();

				// reset positio to start position
				position->setPosition(s_position);


				btTransform m_transform;

				m_transform.setFromOpenGLMatrix(&s_position._11);
				rigidBody->setInterpolationWorldTransform(m_transform);
				rigidBody->setCenterOfMassTransform(m_transform);

			}

			physicBody->reset();
			
			physicBody->setVelocity(physicBody->getStartVelocity());

			if (m_world->getBroadphase()->getOverlappingPairCache() != NULL )
				m_world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(rigidBody->getBroadphaseProxy(),m_world->getDispatcher());

		}

	}

}



void PhysicsComponent::update(real frametime, real timestep)
{
	s_activePhysicsComponent= this;
	m_accumulator+= frametime;
	while(m_accumulator >= timestep)
	{
		m_world->stepSimulation(timestep, 2, timestep*0.5f);
		m_accumulator-= timestep;
	}

#ifdef _DEBUG
	if(m_accumulator >= 1.f)
		m_accumulator = frametime;
#endif
}

real PhysicsComponent::getGravityAcceleration() const
{
	return m_gravityAcceleration;
}

void PhysicsComponent::setGravityAcceleration(real gravityAcceleration)
{
	m_gravityAcceleration= gravityAcceleration;
	m_world->setGravity(btVector3(0.f, -getGravityAcceleration(), 0.f));
}

Publisher<CollisionObserver>& PhysicsComponent::getCollisionPublisher(){

	return m_collisionPublisher;
}

void PhysicsComponent::fireCollisionEvent(const ObjectId& idObjA,const ObjectId& idObjB){

	CollisionData data(idObjA,idObjB);
	m_collisionPublisher.RaiseEvent(&CollisionObserver::CollisionEvent,data);

}

void PhysicsComponent::internalTickCallback(btDynamicsWorld* world, real timeStep)
{
	DLOG_ASSERT(s_activePhysicsComponent != NULL);
	s_activePhysicsComponent->internalTickHandler(world, timeStep);
}

void PhysicsComponent::internalTickHandler(btDynamicsWorld* world, real timeStep)
{
	const uint32 numManifolds= world->getDispatcher()->getNumManifolds();
	for(uint32 manifoldIndex= 0; manifoldIndex < numManifolds; ++manifoldIndex)
	{

		btPersistentManifold* manifold= world->getDispatcher()->getManifoldByIndexInternal(manifoldIndex);

		const uint32 numContacts= manifold->getNumContacts();

		btRigidBody* bodyA = static_cast<btRigidBody*>(manifold->getBody0());
		btRigidBody* bodyB = static_cast<btRigidBody*>(manifold->getBody1());

		ObjectId* idBodyA = static_cast<ObjectId*>(bodyA->getUserPointer());
		ObjectId* idBodyB = static_cast<ObjectId*>(bodyB->getUserPointer());

		for(uint32 contactIndex= 0; contactIndex < numContacts; ++contactIndex)
		{
			const btManifoldPoint& contactPoint= manifold->getContactPoint(contactIndex);
			real impulse= contactPoint.getAppliedImpulse();
			if(impulse > 0.f)
			{
				fireCollisionEvent(*idBodyA,*idBodyB);

				//raise event
			}
//			contactPoint.m_localPointA.getX(), contactPoint.m_localPointA.getY(), contactPoint.m_localPointA.getZ()
//			contactPoint.m_positionWorldOnA.getX(),contactPoint.m_positionWorldOnA.getY(), contactPoint.m_positionWorldOnA.getZ()
// 			bodyA->getLinearVelocity().getX(), bodyA->getLinearVelocity().getY(), bodyA->getLinearVelocity().getZ()
// 			-contactPoint.m_normalWorldOnB.getX(), -contactPoint.m_normalWorldOnB.getY(), -contactPoint.m_normalWorldOnB.getZ()
// 			contactPoint.getAppliedImpulse(),
		}
	}
}
