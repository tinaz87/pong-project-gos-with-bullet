#include "collisionObserver.h"

CollisionData::CollisionData(const ObjectId& objA, const ObjectId& objB)
	:m_objIdA(objA)
	,m_objIdB(objB)
{

}

CollisionData::~CollisionData()
{

}

const ObjectId& CollisionData::getObjectIdA() const 
{
	return m_objIdA;
}

const ObjectId& CollisionData::getObjectIdB() const
{
	return m_objIdB;
}

void CollisionData::setObjectIdA(const ObjectId& objA)
{
	m_objIdA= objA;
}

void CollisionData::setObjectIdB(const ObjectId& objB)
{
	m_objIdB= objB;
}

