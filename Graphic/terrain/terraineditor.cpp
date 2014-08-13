
#include "stdafx.h"
#include "terraineditor.h"
#include <objidl.h>
#include <gdiplus.h> 

using namespace Gdiplus;
using namespace graphic;


cTerrainEditor::cTerrainEditor() : 
	m_numLayer(0)
{
	InitLayer();

	//m_shader.Create( "../../media/shader/hlsl_terrain_splatting.fx", "TShader" );
	m_emptyTexture.Create( "../../media/texture/emptyTexture.png" );
}

cTerrainEditor::~cTerrainEditor()
{

}


void cTerrainEditor::Render()
{
	if (m_numLayer > 0)
	{
	}
	else
	{
		cTerrain::Render();
	}
}


void cTerrainEditor::RenderShader(cShader &shader)
{
	if (m_numLayer > 0)
	{
		shader.SetTexture( "SplattingAlphaMap", m_alphaTexture );
		shader.SetFloat( "alphaUVFactor", GetTextureUVFactor() );

		const string texName[] = {"Tex1", "Tex2", "Tex3", "Tex4" };
		for (int i=0; i < m_numLayer; ++i)
			shader.SetTexture( texName[ i], *m_layer[ i].texture );
		for (int i=m_numLayer; i < MAX_LAYER; ++i)
			shader.SetTexture( texName[ i], m_emptyTexture );

		shader.SetRenderPass(3);
		cTerrain::RenderShader(shader);
	}
	else
	{
		cTerrain::RenderShader(shader);
	}
}


void cTerrainEditor::Brush( const cTerrainCursor &cursor )
{
	RET(!cursor.GetBrushTexture());

	if (GetTopLayer().texture != cursor.GetBrushTexture())
	{
		// 빈 레이어 일 때만 새 레이어를 추가한다.
		if (GetTopLayer().texture && !AddLayer())
			return; // 더 이상 추가할 레이어가 없다면 종료한다.
	}

	sSplatLayer &curLayer = GetTopLayer();
	curLayer.texture = (cTexture*)cursor.GetBrushTexture();
	const int MASK = ~(0xFF << (24 - (curLayer.layer * 8)));

	float u, v;
	GetTextureUV(cursor.GetCursorPos(), u, v);

	const float WIDTH = GetTerrainWidth();
	const float HEIGHT = GetTerrainHeight();
	
	D3DLOCKED_RECT lockrect;
	m_alphaTexture.Lock(lockrect);

	BYTE *pbits = (BYTE*)lockrect.pBits;
	for (int ay=0; ay < ALPHA_TEXTURE_SIZE_H; ++ay)
	{
		for (int ax=0; ax < ALPHA_TEXTURE_SIZE_W; ++ax)
		{
			const float au = (float)ax / (float)(ALPHA_TEXTURE_SIZE_W - 1);
			const float av = (float)ay / (float)(ALPHA_TEXTURE_SIZE_H - 1);

			const float ru = fabs(au-u) * WIDTH;
			const float rv = fabs(av-v) * HEIGHT;
			const float len = sqrt( ru*ru + rv*rv );
			//float len = sqrt( ((au-u)*(au-u)) + ((av-v)*(av-v)) );
			//len *= (COL_CELL_COUNT * CELL_SIZE);

			// A8R8G8B8 Format
			DWORD *ppixel = (DWORD*)(pbits + (ax*4) + (lockrect.Pitch * ay));

			if (len <= cursor.GetInnerBrushRadius())
			{
				int color = (int)(255.f * cursor.GetInnerBrushAlpha());
				color = color << (24 - (curLayer.layer * 8));

				*ppixel = color | (*ppixel & MASK);
			}
			else if (len <= cursor.GetOuterBrushRadius())
			{
				// 보간
				const float w = cursor.GetOuterBrushRadius() - cursor.GetInnerBrushRadius();
				const float delta = 1.f - ((len - cursor.GetInnerBrushRadius()) / w);
				int color = (int)(cursor.GetInnerBrushAlpha() * delta * 255.f);

				const int dest = (*ppixel >> (24 - (curLayer.layer * 8))) & 0xFF;
				if (color > dest)
				{
					color = color << (24 - (curLayer.layer * 8));
					*ppixel = color | (*ppixel & MASK);
				}
			}
		}
	}

	m_alphaTexture.Unlock();
}


void cTerrainEditor::InitLayer()
{
	m_numLayer = 0;
	for (int i=0; i < MAX_LAYER; ++i)
	{
		m_layer[ i].layer = i;
		m_layer[ i].texture = NULL;
	}

	m_alphaTexture.Clear();
	m_alphaTexture.Create( ALPHA_TEXTURE_SIZE_W, ALPHA_TEXTURE_SIZE_H,
		D3DFMT_A8R8G8B8 );
}


// 최상위 레이어 리턴
sSplatLayer& cTerrainEditor::GetTopLayer()
{
	if (m_numLayer <= 0)
		m_numLayer = 1;
	return m_layer[ m_numLayer - 1];
}


// 레이어 추가.
bool cTerrainEditor::AddLayer()
{
	if (m_numLayer >= MAX_LAYER)
		return false;

	++m_numLayer;
	return true;
}


void cTerrainEditor::GetTextureUV(const Vector3 &pos, OUT float &tu, OUT float &tv)
{
	const float width = GetColCellCount() * GetCellSize();
	const float height = GetRowCellCount() * GetCellSize();

	float x = pos.x + width/2.f;
	float z = pos.z + height/2.f;
	z = height - z;

	tu = x / (float)width;
	tv = z / (float)height;
}


void cTerrainEditor::Clear()
{
	cTerrain::Clear();
	InitLayer();

}


// heightFactor 값을 수정한다.
// 이미 지형이 로딩된 상태라면, 높이 값을 heightFactor 값에 맞춰
// 수정한다.
void cTerrainEditor::SetHeightFactor(const float heightFactor)
{
	m_heightFactor = heightFactor;
	if (m_colCellCount <= 0)
		return; // 아직 지형이 로딩되지 않아서 리턴한다.
	
	const wstring wfileName = common::str2wstr(m_heightMapFileName);
	Bitmap bmp(wfileName.c_str());
	if (Ok != bmp.GetLastStatus())
		return;

	const int VERTEX_COL_COUNT = m_colCellCount + 1;
	const int VERTEX_ROW_COUNT = m_rowCellCount + 1;
	const float WIDTH = m_colCellCount * m_cellSize;
	const float HEIGHT = m_rowCellCount * m_cellSize;

	const float incX = (float)(bmp.GetWidth()-1) / (float)m_colCellCount;
	const float incY = (float)(bmp.GetHeight()-1) /(float) m_rowCellCount;

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();

	for (int i=0; i < VERTEX_COL_COUNT; ++i)
	{
		for (int k=0; k < VERTEX_ROW_COUNT; ++k)
		{
			sVertexNormTex *vtx = pv + (k*VERTEX_COL_COUNT) + i;

			Color color;
			bmp.GetPixel(i*incX, k*incY, &color);
			const float h = ((color.GetR() + color.GetG() + color.GetB()) / 3.f) 
				* heightFactor;
			vtx->p.y = h;
		}
	}

	m_grid.GetVertexBuffer().Unlock();

	m_grid.CalculateNormals();
}


// textureUVFactor 값을 수정한다.
// 이미 지형이 로딩된 상태라면, UV 값을 textureUVFactor 값에 맞춰
// 수정한다.
void cTerrainEditor::SetTextureUVFactor(const float textureUVFactor)
{
	m_textureUVFactor = textureUVFactor;
	m_grid.SetTextureUVFactor(textureUVFactor);
}
