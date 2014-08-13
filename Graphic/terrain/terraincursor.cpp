
#include "StdAfx.h"
#include "TerrainCursor.h"


using namespace graphic;

cTerrainCursor::cTerrainCursor() :
	m_innerRadius(10)
,	m_outerRadius(60)
,	m_innerAlpha(1.f)
,	m_brushTexture(NULL)
{
	m_innerCircle.Create( CURSOR_VERTEX_COUNT, sizeof(sVertexDiffuse), sVertexDiffuse::FVF );
	m_outerCircle.Create( CURSOR_VERTEX_COUNT, sizeof(sVertexDiffuse), sVertexDiffuse::FVF );

}


cTerrainCursor::~cTerrainCursor()
{
}


void cTerrainCursor::Render()
{
	m_innerCircle.RenderLineStrip();
	m_outerCircle.RenderLineStrip();
}


void cTerrainCursor::UpdateCursor( graphic::cTerrain &terrain,  const Vector3 &cursorPos )
{
	m_pos = cursorPos;

	sVertexDiffuse *innerVertices = (sVertexDiffuse*)m_innerCircle.Lock();	
	sVertexDiffuse *outerVertices = (sVertexDiffuse*)m_outerCircle.Lock();	
	
	float angle = 0.f;
	int index = 0;
	const float offset = MATH_PI*2.f / (float)(CURSOR_VERTEX_COUNT-1);

	while ((angle < MATH_PI*2.f) && (index < CURSOR_VERTEX_COUNT))
	{
		Vector3 inner(m_innerRadius*cos(angle), 0.f, m_innerRadius*sin(angle));
		inner += cursorPos;
		innerVertices[ index].p = inner;
		innerVertices[ index].p.y = terrain.GetHeight(inner.x, inner.z);
		innerVertices[ index].c = D3DXCOLOR( 1, 0, 0, 0 );

		Vector3 outer(m_outerRadius*cos(angle), 0.f, m_outerRadius*sin(angle));
		outer += cursorPos;
		outerVertices[ index].p = outer;
		outerVertices[ index].p.y = terrain.GetHeight(outer.x, outer.z);
		outerVertices[ index].c = D3DXCOLOR( 1, 0, 0, 0 );

		index++;
		angle += offset;
	}

	m_innerCircle.Unlock();
	m_outerCircle.Unlock();
}


// 텍스쳐 브러쉬를 선택한다.
void cTerrainCursor::SelectBrushTexture(const string &fileName )
{
	if (fileName.empty())
	{
		m_brushTexture = NULL;
		return;
	}

	m_brushTexture = cResourceManager::Get()->LoadTexture(fileName);
	if (!m_brushTexture)
	{
		MessageBoxA(NULL, "텍스쳐 파일을 읽을 수 없습니다.", "ERROR", MB_OK);
	}
}
