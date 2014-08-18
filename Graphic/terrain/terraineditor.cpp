
#include "stdafx.h"
#include "terraineditor.h"
#include <objidl.h>
#include <gdiplus.h> 

using namespace Gdiplus;
using namespace graphic;



cTerrainEditor::cTerrainEditor()
	//m_numLayer(0)
//,	m_layer(MAX_LAYER)
{
	InitLayer();

	//m_shader.Create( "../../media/shader/hlsl_terrain_splatting.fx", "TShader" );
	m_emptyTexture.Create( "../../media/texture/emptyTexture.png" );
}

cTerrainEditor::~cTerrainEditor()
{

}


// 지형 정보를 저장한다.  알파 텍스쳐 제외.
bool cTerrainEditor::SaveTerrain( const string &fileName )
{
	return m_grid.WriteGridFile(fileName);
}


// 지형 파일 로드
bool cTerrainEditor::LoadTerrain( const string &fileName )
{
	return m_grid.ReadGridFromFile(fileName);
}


// 지형정보를 토대로 지형을 생성한다.
bool cTerrainEditor::LoadTerrain( const sRawTerrain &rawTerrain )
{
	Clear();

	const string mediaDir = cResourceManager::Get()->GetMediaDirectory();

	if (rawTerrain.heightMapStyle == 0)
	{
		// 높이맵으로 만들어진 지형이면, 높이 맵을 로딩한다.
		if (rawTerrain.heightMap.empty())
		{
			CreateTerrain( rawTerrain.rowCellCount, rawTerrain.colCellCount, 
				rawTerrain.cellSize, rawTerrain.textureFactor );
			CreateTerrainTexture( mediaDir+rawTerrain.bgTexture );
		}
		else
		{
			// 기본 지형에서 만들어진 지형이면, 기본 지형을 생성한다.
			CreateFromHeightMap( mediaDir+rawTerrain.heightMap, mediaDir+rawTerrain.bgTexture, 
				rawTerrain.heightFactor, rawTerrain.textureFactor, 
				rawTerrain.rowCellCount, rawTerrain.colCellCount, rawTerrain.cellSize );
		}
	}
	else if (rawTerrain.heightMapStyle == 1)
	{
		CreateFromGRDFormat(mediaDir+rawTerrain.heightMap, mediaDir+rawTerrain.bgTexture, 
			rawTerrain.heightFactor, rawTerrain.textureFactor, 
			rawTerrain.rowCellCount, rawTerrain.colCellCount, rawTerrain.cellSize );
	}


	// 레이어 생성
	for (int i=0; i < MAX_LAYER; ++i)
	{
		if (rawTerrain.layer[ i].texture.empty())
			break;

		AddLayer();
		m_layer[ i].texture = cResourceManager::Get()->LoadTexture( 
			mediaDir+rawTerrain.layer[ i].texture );
	}

	// 모델 생성.
	for (u_int i=0; i < rawTerrain.models.size(); ++i)
	{
		if (cModel *model = AddRigidModel(mediaDir+rawTerrain.models[ i].fileName))
		{
			model->SetTM(rawTerrain.models[ i].tm);
		}
	}

	m_alphaTexture.Create( mediaDir+rawTerrain.alphaTexture );

	return true;
}


// 지형정보를 sRawTerrain에 저장한다.
void cTerrainEditor::GenerateRawTerrain( OUT sRawTerrain &out )
{
	out.rowCellCount = m_rowCellCount;
	out.colCellCount = m_colCellCount;
	out.cellSize = m_cellSize;
	out.heightMapStyle = 1;//(m_heightMapFileName.empty()? 1 : 0);
	out.heightMap = cResourceManager::Get()->GetRelativePathToMedia(m_heightMapFileName);
	out.bgTexture = cResourceManager::Get()->GetRelativePathToMedia(m_grid.GetTexture().GetTextureName());
	out.textureFactor = m_textureUVFactor;
	out.heightFactor = m_heightFactor;

	for (u_int i=0; i < m_layer.size(); ++i)
	{
		if (m_layer[ i].texture)
			out.layer[ i].texture = cResourceManager::Get()->GetRelativePathToMedia(m_layer[ i].texture->GetTextureName());
	}

	out.models.reserve( m_rigids.size() );
	for (u_int i=0; i < m_rigids.size(); ++i)
	{
		out.models.push_back( sRawTerrrainModel() );
		out.models.back().fileName = cResourceManager::Get()->GetRelativePathToMedia(m_rigids[ i]->GetFileName());
		out.models.back().tm = m_rigids[ i]->GetTM();
	}

}


void cTerrainEditor::Render()
{
	if (m_layer.empty())
	{
		cTerrain::Render();
	}
	else
	{
		
	}
}


void cTerrainEditor::RenderShader(cShader &shader)
{
	if (m_layer.empty())
	{
		cTerrain::RenderShader(shader);
	}
	else
	{
		shader.SetTexture( "SplattingAlphaMap", m_alphaTexture );
		shader.SetFloat( "alphaUVFactor", GetTextureUVFactor() );

		const string texName[] = {"Tex1", "Tex2", "Tex3", "Tex4" };
		for (u_int i=0; i < m_layer.size(); ++i)
			shader.SetTexture( texName[ i], *m_layer[ i].texture );
		for (u_int i=m_layer.size(); i < MAX_LAYER; ++i)
			shader.SetTexture( texName[ i], m_emptyTexture );

		shader.SetRenderPass(3);
		cTerrain::RenderShader(shader);
	}
}


// 지형의 높낮이를 조절한다.
void cTerrainEditor::BrushTerrain( const cTerrainCursor &cursor, const float elapseT )
{
	if (m_colCellCount <= 0)
		return; // 아직 지형이 로딩되지 않아서 리턴한다.

	const int VERTEX_COL_COUNT = m_colCellCount + 1;
	const int VERTEX_ROW_COUNT = m_rowCellCount + 1;

	const Vector3 cursorPos = cursor.GetCursorPos();

	if (sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock())
	{
		for (int i=0; i < VERTEX_COL_COUNT; ++i)
		{
			for (int k=0; k < VERTEX_ROW_COUNT; ++k)
			{
				sVertexNormTex *vtx = pv + (k*VERTEX_COL_COUNT) + i;

				Vector3 v = vtx->p - cursorPos;
				const float length = v.Length();
				if (cursor.GetOuterBrushRadius() < length)
					continue;

				float offsetH = 0.f;
				if (cursor.GetTerrainEditMode() == TERRAIN_EDIT_MODE::UP)
				{
					offsetH = cursor.GetBrushSpeed() * elapseT;
				}
				else if (cursor.GetTerrainEditMode() == TERRAIN_EDIT_MODE::DOWN)
				{
					offsetH = -cursor.GetBrushSpeed() * elapseT;
				}

				vtx->p.y += offsetH * (cursor.GetOuterBrushRadius() - length) / cursor.GetOuterBrushRadius();
			}
		}

		m_grid.GetVertexBuffer().Unlock();
	}

	m_grid.CalculateNormals();
}


// 지형에 텍스쳐를 입힌다.
void cTerrainEditor::BrushTexture( const cTerrainCursor &cursor )
{
	RET(!cursor.GetBrushTexture());

	if (GetTopLayer().texture != cursor.GetBrushTexture())
	{
		// 빈 레이어 일 때만 새 레이어를 추가한다.
		if (GetTopLayer().texture && !AddLayer())
			return; // 더 이상 추가할 레이어가 없다면 종료한다.
	}

	sSplatLayer &curLayer = GetTopLayer();
	const int layerIdx = m_layer.size()-1;
	curLayer.texture = (cTexture*)cursor.GetBrushTexture();
	//const int MASK = ~(0xFF << (24 - (layerIdx * 8)));
	const int MASK = ~GetAlphaMask(layerIdx);

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
				if (cursor.IsEraseMode()) // 지우개 모드일 때는 역의 값을 넣게한다.
					color = 255-color;
				color = color << (24 - (layerIdx * 8));

				*ppixel = color | (*ppixel & MASK);
			}
			else if (len <= cursor.GetOuterBrushRadius())
			{
				// 보간
				const float w = cursor.GetOuterBrushRadius() - cursor.GetInnerBrushRadius();
				const float delta = 1.f - ((len - cursor.GetInnerBrushRadius()) / w);
				int color = (int)(cursor.GetInnerBrushAlpha() * delta * 255.f);
				if (cursor.IsEraseMode()) // 지우개 모드일 때는 역의 값을 넣게한다.
					color = 255-color;

				const int dest = (*ppixel >> (24 - (layerIdx * 8))) & 0xFF;

				if ( (cursor.IsEraseMode() && (color < dest)) ||
					(!cursor.IsEraseMode() && (color > dest)))
				{
					color = color << (24 - (layerIdx * 8));
					*ppixel = color | (*ppixel & MASK);
				}
			}
		}
	}

	m_alphaTexture.Unlock();
}


void cTerrainEditor::InitLayer()
{
	m_layer.clear();

	m_alphaTexture.Clear();
	m_alphaTexture.Create( ALPHA_TEXTURE_SIZE_W, ALPHA_TEXTURE_SIZE_H,
		D3DFMT_A8R8G8B8 );
}


// 최상위 레이어 리턴
sSplatLayer& cTerrainEditor::GetTopLayer()
{
	if (m_layer.empty())
		m_layer.push_back(sSplatLayer());
	return m_layer.back();
}


// 레이어 추가.
bool cTerrainEditor::AddLayer()
{
	if (m_layer.size() >= MAX_LAYER)
		return false;

	m_layer.push_back(sSplatLayer());
	return true;
}


// layer 에 해당하는 마스크를 리턴한다.
DWORD cTerrainEditor::GetAlphaMask(const int layer)
{
	return (0xFF << (24 - (layer * 8)));
}


// layer 위치의 레이어를 제거하고, 나머지는 밀어 올린다.
void cTerrainEditor::DeleteLayer(int layer)
{
	RET(m_layer.empty());

	common::rotatepopvector(m_layer, (u_int)layer);

	// 비어있는 알파 이미지도 같이 밀어올린다.
	const DWORD delMask = GetAlphaMask(layer);
	DWORD moveMask = 0;
	for (u_int i=layer; i < m_layer.size(); ++i)
		moveMask |= GetAlphaMask(i+1);

	D3DLOCKED_RECT lockrect;
	m_alphaTexture.Lock(lockrect);

	BYTE *pbits = (BYTE*)lockrect.pBits;
	for (int ay=0; ay < ALPHA_TEXTURE_SIZE_H; ++ay)
	{
		for (int ax=0; ax < ALPHA_TEXTURE_SIZE_W; ++ax)
		{
			// A8R8G8B8 Format
			DWORD *ppixel = (DWORD*)(pbits + (ax*4) + (lockrect.Pitch * ay));
			DWORD moveVal = *ppixel & moveMask;
			*ppixel = *ppixel & ~(delMask | moveMask); // 이동할 AlphaTexture 초기화
			*ppixel = *ppixel | (moveVal << 8);
		}
	}

	m_alphaTexture.Unlock();

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
