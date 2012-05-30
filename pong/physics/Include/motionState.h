#pragma once
#include <LinearMath/btMotionState.h>
#include "dataTypes.h"

class MotionState : public btMotionState
{
public:
	// CONSTRUCTORS, DESTRUCTORS & INITIALIZERS
	MotionState(const vector3& graphicOriginOffset);
	virtual ~MotionState();

	// GETTERS
	void			getWorldTransformation(vector3* translation, quaternion* rotation) const;
	const matrix&	getGfxWorldTransformation() const;
	const vector3&  getGraphicsOriginOffset() const;

	// SETTERS
	void			setGfxWorldTransformation(const vector3& translation, const quaternion& rotation);
	void			setGfxWorldTransformation(const matrix& gfxTransformation);
	void			setGraphicOriginOffset(const vector3& graphicOriginOffset);

	// btMotionState interface implementation
	virtual void	getWorldTransform(btTransform& centerOfMassWorldTrans) const;
	virtual void	setWorldTransform(const btTransform& centerOfMassWorldTrans);

private:
	matrix		m_physicToGraphic;
	matrix		m_graphicWorldTransformation;
	btTransform	m_bulletWorldTransform;
	vector3		m_graphicsOriginOffset;
	void		updateBulletTransform();

};