
#include "StdAfx.h"
#include "TerrainCursor.h"


using namespace graphic;

cTerrainCursor::cTerrainCursor() :
	m_innerRadius(30)
{
	m_innerCircle.Create( CURSOR_VERTEX_COUNT, sizeof(sVertexDiffuse), sVertexDiffuse::FVF );

}


cTerrainCursor::~cTerrainCursor()
{
}


void cTerrainCursor::Render()
{
	m_innerCircle.RenderLineStrip();
}


void cTerrainCursor::UpdateCursor( graphic::cTerrain &terrain,  const Vector3 &cursorPos )
{
	m_pos = cursorPos;

	sVertexDiffuse *vertices = (sVertexDiffuse*)m_innerCircle.Lock();	
	
	float angle = 0.f;
	int index = 0;
	const float offset = MATH_PI*2.f / (float)CURSOR_VERTEX_COUNT;

	while ((angle < MATH_PI*2.f) && (index < CURSOR_VERTEX_COUNT))
	{
		Vector3 inner(m_innerRadius*cos(angle), 0.f, m_innerRadius*sin(angle));
		inner += cursorPos;
		vertices[ index].p = inner;
		vertices[ index].p.y = terrain.GetHeight(inner.x, inner.z) + 0.2f;
		vertices[ index].c = D3DXCOLOR( 1, 0, 0, 0 );

		index++;
		angle += offset;
	}

	m_innerCircle.Unlock();
}
