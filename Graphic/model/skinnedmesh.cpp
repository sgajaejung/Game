
#include "stdafx.h"
#include "skinnedmesh.h"

using namespace graphic;


cSkinnedMesh::cSkinnedMesh(const int id, vector<Matrix44> *palette, const sRawMesh &raw) : 
	cMesh(id, raw)
,	m_rawMesh(raw)
,	m_palette(palette)
,	m_skinnMeshBuffer(NULL)
{

	// 셰이더스키닝을 못하는 상황일 때는 소프트웨어 스키닝을 
	// 하기 위해서 버퍼를 동적으로 생성한다.
	if (m_palette && (64 < m_palette->size()))
	{
		m_skinnMeshBuffer = new cMeshBuffer(raw);
		SetMeshBuffer(m_skinnMeshBuffer);
	}

}

cSkinnedMesh::~cSkinnedMesh()
{
	SAFE_DELETE(m_skinnMeshBuffer);
}


void cSkinnedMesh::Render(const Matrix44 &parentTm)
{
	if (m_shader && m_palette && (m_palette->size() < 64))
	{
		ApplyPaletteShader(*m_shader);
		cMesh::RenderShader(*m_shader, parentTm);
	}
	else
	{
		ApplyPalette();
		cMesh::Render(parentTm);
	}
}

void cSkinnedMesh::RenderShader( cShader &shader, const Matrix44 &parentTm )
{
	ApplyPaletteShader(shader);
	cMesh::RenderShader(shader, parentTm);
}


void cSkinnedMesh::RenderShadow(cShader &shader, const Matrix44 &parentTm)
{
	ApplyPaletteShader(shader);
	cMesh::RenderShadow(shader, parentTm);
}


// 팔레트 적용.
void cSkinnedMesh::ApplyPalette()
{
	RET(!m_palette);

	// 소프트웨어 스키닝일 때 동적으로 할당된 버퍼가 없다면 새로 생성한다.
	if (!m_skinnMeshBuffer)
	{
		m_skinnMeshBuffer = new cMeshBuffer(m_rawMesh);
		SetMeshBuffer(m_skinnMeshBuffer);
	}


	cVertexBuffer &vtxBuffer = m_skinnMeshBuffer->GetVertexBuffer();
	if (sVertexNormTexSkin *vertices = (sVertexNormTexSkin*)vtxBuffer.Lock())
	{
		BOOST_FOREACH (const sVertexWeight &weight, m_rawMesh.weights)
		{
			const int vtxIdx = weight.vtxIdx;
			vertices[ vtxIdx].p = Vector3(0,0,0);
			vertices[ vtxIdx].n = Vector3(0,0,0);

			for( int k=0; k < weight.size; ++k )
			{
				const sWeight *w = &weight.w[ k];
				Vector3 v = m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ w->bone];
				Vector3 n = m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ w->bone] );
				vertices[ vtxIdx].p += v * w->weight;
				vertices[ vtxIdx].n += n * w->weight;
			}
		}
	}

	vtxBuffer.Unlock();
}


// 팔레트 적용.
void cSkinnedMesh::ApplyPaletteShader(cShader &shader)
{
	RET(!m_palette);

	const int paletteSize = min(64, m_palette->size());
	shader.SetMatrixArray("mPalette", (Matrix44*)&(*m_palette)[0], paletteSize);
}
