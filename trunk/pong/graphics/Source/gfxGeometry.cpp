#include "gfxGeometry.h"

const StringHash GfxGeometry::GFX_GEOMETRY_ID= StringHash("gfxGeometry");

GfxGeometry::GfxGeometry(const ObjectId& objectId)
	:ObjectProperty(GFX_GEOMETRY_ID, objectId)
{
}

GfxGeometry::~GfxGeometry()
{
	m_indices.clear();
}

void GfxGeometry::setVertices(const vector4* vertices, uint32 verticesCount)
{
	bool resetTextureCoord= false;
	if(m_textureCoord.size() != verticesCount)
	{
		m_textureCoord.resize(verticesCount);
		resetTextureCoord= true;
	}
	m_vertices.resize(verticesCount);
	for(uint32 i= 0; i < verticesCount; ++i)
	{
		m_vertices[i]= vertices[i];
		if(resetTextureCoord)
		{
			m_textureCoord[i].x= 0.f;
			m_textureCoord[i].y= 0.f;
		}
	}
}

void GfxGeometry::setTriangles(const uint32* indices, uint32 indicesCount)
{
	m_indices.resize(indicesCount);
	for(uint32 i= 0; i < indicesCount; ++i)
	{
		m_indices[i]= indices[i];
	}
}

void GfxGeometry::setTextureCoord(const vector2* textureCoord, uint32 verticesCount)
{
	assert(m_vertices.size() == verticesCount);
	m_textureCoord.resize(verticesCount);
	for(uint32 i= 0; i < verticesCount; ++i)
	{
		m_textureCoord[i]= textureCoord[i];
	}
}

vector4* GfxGeometry::editVertices()
{
	return m_vertices.data();
}

const vector4* GfxGeometry::getVertices() const
{
	return m_vertices.data();
}

uint32* GfxGeometry::editIndices()
{
	return m_indices.data();
}

const uint32* GfxGeometry::getIndices() const
{
	return m_indices.data();
}

uint32	GfxGeometry::getVerticesCount() const
{
	return m_vertices.size();
}

uint32	GfxGeometry::getIndicesCount() const
{
	return m_indices.size();
}

vector2* GfxGeometry::editTextureCoord()
{
	return m_textureCoord.data();
}

const vector2* GfxGeometry::getTextureCoord() const
{
	return m_textureCoord.data();
}