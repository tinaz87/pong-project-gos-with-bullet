#pragma once
#include "ObjectProperty.h"
#include "Allocator.h"

class GfxGeometry: public ObjectProperty
{
public:
	static const StringHash GFX_GEOMETRY_ID;
	GfxGeometry(const ObjectId& objectId);
	~GfxGeometry();
	void			setVertices(const vector4* vertices, uint32 verticesCount);
	void			setTriangles(const uint32* indices, uint32 indicesCount);
	void			setTextureCoord(const vector2* textureCoord, uint32 verticesCount);
	
	vector4*		editVertices();
	const vector4*	getVertices() const;
	
	uint32*			editIndices();
	const uint32*	getIndices() const;
	
	uint32			getVerticesCount() const;
	uint32			getIndicesCount() const;
	
	vector2*		editTextureCoord();
	const vector2*	getTextureCoord() const;
private:
	MvVector<vector4>	m_vertices;    //must be aligned
	MvVector<vector2>	m_textureCoord;//must be aligned
	MvVector<uint32>	m_indices;
};