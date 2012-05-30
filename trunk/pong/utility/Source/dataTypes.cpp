#include "dataTypes.h"

#ifdef USE_DX
	matrix MatrixRotationFromQuaternion(const quaternion& rotation)
{
	return XMMatrixRotationQuaternion(rotation);
}

//if the input matrix contains scales, shears, or other non-rotation transformations 
//in the upper 3x3 matrix, then the output of this function is ill-defined
quaternion QuaternionFromRotationMatrix(const matrix& rotationMatrix)
{
	return XMQuaternionRotationMatrix(rotationMatrix);
}

void SetMatrixTranslation(matrix* matrix, const vector3& translation)
{
	*matrix= XMMatrixTranslation(translation.x, translation.y, translation.z);
}

void GetTranslation(const matrix& positionMatrix, vector3* translation)
{
	translation->x= XMVectorGetX(positionMatrix.r[3]);
	translation->y= XMVectorGetY(positionMatrix.r[3]);
	translation->z= XMVectorGetZ(positionMatrix.r[3]);
}

real Vector3LenghtSq(const vector3& vector)
{
	XMVECTOR tmp= XMLoadFloat3(&vector);
	return XMVectorGetX(XMVector3LengthSq(tmp));
}

quaternion InitQuaternion(real x, real y, real z, real w)
{
	return XMVectorSet(x, y, z, w);
}

matrix MatrixPosition(const vector3& translation, const quaternion& rotation)
{
	matrix matRot;
	matrix matTx;
	matRot= MatrixRotationFromQuaternion(rotation);
	SetMatrixTranslation(&matTx, translation);
	matRot= matRot * matTx;
	return matRot;
}

void  AddTranslation(matrix* matrix, const vector3& translation)
{
	vector4 trl= XMLoadFloat3(&translation);
	matrix->r[3]= _mm_add_ps(matrix->r[3], trl);
}
#endif 