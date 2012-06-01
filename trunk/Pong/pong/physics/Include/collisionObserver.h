#pragma once
#include "ObjectId.h"

class CollisionData
{
public:
	CollisionData(const ObjectId& objA, const ObjectId& objB);
	~CollisionData();

	const ObjectId& getObjectIdA() const;
	const ObjectId& getObjectIdB() const;
	void setObjectIdA(const ObjectId& objA) ;
	void setObjectIdB(const ObjectId& objB);

private:
	ObjectId m_objIdA;
	ObjectId m_objIdB;
};


class CollisionObserver
{
public:
	virtual ~CollisionObserver() {}
	virtual void CollisionEvent(const CollisionData& collision)= 0;
};