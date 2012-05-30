#pragma once

typedef __int8				int8;
typedef __int16				int16;
typedef __int32				int32;
typedef __int64				int64;
typedef unsigned __int8		uint8;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;

#define uint8MAX  0xffui8
#define uint16MAX 0xffffui16
#define uint32MAX 0xffffffffui32
#define uint64MAX 0xffffffffffffffffui64


//see bullet physics engine 
#if defined(USE_DOUBLE_PRECISION)
typedef double real;
#define BT_LARGE_FLOAT 1e30
#else
typedef float real;
#define BT_LARGE_FLOAT 1e18f
#endif

// A structure for our custom vertex type
struct VERTEXCOLOR
{
	float x, y, z;		// The transformed position for the vertex
	unsigned long color;	// The vertex color
};

// A structure for vertex with texture coord
struct VERTEXTEX
{
	float x, y, z;		// The transformed position for the vertex
	unsigned long color;// The vertex color
	float tu, tv;		// The texture coordinates
};

#define PI 3.14159265358979323f

#ifdef USE_DX
//see http://msdn.microsoft.com/en-us/library/windows/desktop/ff729728%28v=vs.85%29.aspx
#include <windows.h>
#include "xnamath.h"

//xnamath wrapper
typedef XMVECTOR vector4;
typedef XMFLOAT3 vector3;
typedef XMFLOAT2 vector2;
typedef XMMATRIX matrix;
typedef XMVECTOR quaternion;

static const matrix MAT_IDENTITY= XMMatrixIdentity();

matrix     MatrixRotationFromQuaternion(const quaternion& rotation);
//if the input matrix contains scales, shears, or other non-rotation transformations 
//in the upper 3x3 matrix, then the output of this function is ill-defined
quaternion QuaternionFromRotationMatrix(const matrix& rotationMatrix);
quaternion InitQuaternion(real x, real y, real z, real w);
void	   SetMatrixTranslation(matrix* matrix, const vector3& translation);
void	   AddTranslation(matrix* matrix, const vector3& translation);
void       GetTranslation(const matrix& positionMatrix, vector3* translation);
real	   Vector3LenghtSq(const vector3& vector);
matrix	   MatrixPosition(const vector3& translation, const quaternion& rotation);

#endif
