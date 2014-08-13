
#include "stdafx.h"
#include "terraineditor.h"

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

				*ppixel |= color;
			}
			else if (len <= cursor.GetOuterBrushRadius())
			{
				// 보간
				const float w = cursor.GetOuterBrushRadius() - cursor.GetInnerBrushAlpha();
				const float delta = 1.f - ((len - cursor.GetInnerBrushRadius()) / w);
				int color = (int)(cursor.GetInnerBrushAlpha() * delta * 255.f);

				const int dest = (*ppixel >> (24 - (curLayer.layer * 8))) & 0xFF;
				if (color > dest)
				{
					color = color << (24 - (curLayer.layer * 8));
					*ppixel |= color;
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
