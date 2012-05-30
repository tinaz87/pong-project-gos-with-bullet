#include "motionState.h"

MotionState::MotionState(const vector3& graphicOriginOffset)
	:m_graphicsOriginOffset(0.f, 0.f, 0.f)
	,m_physicToGraphic(MAT_IDENTITY)
	,m_graphicWorldTransformation(MAT_IDENTITY)
	,m_bulletWorldTransform(btMatrix3x3(1.f, 0.f, 0.f,
										0.f, 1.f, 0.f,
										0.f, 0.f, 1.f))
{
	setGraphicOriginOffset(graphicOriginOffset);
}

MotionState::~MotionState()
{
}

void MotionState::getWorldTransform(btTransform& centerOfMassWorldTrans) const
{
	centerOfMassWorldTrans = m_bulletWorldTransform;
}

void MotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
{
	m_bulletWorldTransform = centerOfMassWorldTrans;
	m_bulletWorldTransform.getOpenGLMatrix(&m_graphicWorldTransformation._11);
	m_graphicWorldTransformation = m_physicToGraphic * m_graphicWorldTransformation;
}

const matrix& MotionState::getGfxWorldTransformation() const
{
	return m_graphicWorldTransformation;
}

void MotionState::getWorldTransformation(vector3* translation, quaternion* rotation) const
{
	translation->x= m_graphicWorldTransformation._41/m_graphicWorldTransformation._44;
	translation->y= m_graphicWorldTransformation._42/m_graphicWorldTransformation._44;
	translation->z= m_graphicWorldTransformation._43/m_graphicWorldTransformation._44;

	*rotation= QuaternionFromRotationMatrix(m_graphicWorldTransformation);
}

void MotionState::setGfxWorldTransformation(const vector3& translation, const quaternion& rotation)
{
	m_graphicWorldTransformation= MatrixRotationFromQuaternion(rotation);
	SetMatrixTranslation(&m_graphicWorldTransformation, translation);
	setGfxWorldTransformation(m_graphicWorldTransformation);
}

void MotionState::setGfxWorldTransformation(const matrix& gfxTransformation)
{
	m_graphicWorldTransformation= gfxTransformation;
	updateBulletTransform();
	m_graphicWorldTransformation= m_physicToGraphic * m_graphicWorldTransformation;
}

void MotionState::updateBulletTransform()
{
	m_bulletWorldTransform.setFromOpenGLMatrix(&m_graphicWorldTransformation._11);
}

void MotionState::setGraphicOriginOffset(const vector3& graphicOriginOffset)
{
	m_graphicsOriginOffset= graphicOriginOffset;
	m_physicToGraphic= MAT_IDENTITY;
	SetMatrixTranslation(&m_physicToGraphic, graphicOriginOffset);
}

const vector3& MotionState::getGraphicsOriginOffset() const
{
	return m_graphicsOriginOffset;
}

