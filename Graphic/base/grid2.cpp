
#include "stdafx.h"
#include "grid2.h"

using namespace graphic;


cGrid2::cGrid2()
{
	m_mtrl.InitWhite();
}

cGrid2::~cGrid2()
{

}


void cGrid2::Create( const int rowCellCount, const int colCellCount, const float cellSize,
	const float textureUVFactor)
{
	// init member
	m_rowCellCount = rowCellCount;
	m_colCellCount = colCellCount;
	m_cellSize = cellSize;

	// Init Grid
	const int rowVtxCnt  = rowCellCount+1;
	const int colVtxCnt  = colCellCount+1;
	const int cellCnt = rowCellCount * colCellCount;
	const int vtxCount= rowVtxCnt * colVtxCnt;

	m_vtxBuff.Create( vtxCount, sizeof(sVertexNormTex), sVertexNormTex::FVF);
	{
		sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
		const float startx = -cellSize*(rowCellCount/2);
		const float starty = cellSize*(colCellCount/2);
		const float endx = startx + cellSize*rowCellCount;
		const float endy = starty - cellSize*colCellCount;

		const float uCoordIncrementSize = 1.0f / (float)colCellCount * textureUVFactor;
		const float vCoordIncrementSize = 1.0f / (float)rowCellCount * textureUVFactor;

		int i=0;
		for (float y=starty; y >= endy; y -= cellSize, ++i)
		{
			int k=0;
			for (float x=startx; x <= endx; x += cellSize, ++k )
			{
				int index = (i * colVtxCnt) + k;
				vertices[ index].p = Vector3(x, 0.1f, y);
				vertices[ index].n = Vector3(0,1,0);
				vertices[ index].u = (float)k*uCoordIncrementSize;
				vertices[ index].v = (float)i*vCoordIncrementSize;
			}
		}
		m_vtxBuff.Unlock();
	}


	m_idxBuff.Create( cellCnt*2 );
	{
		WORD *indices = (WORD*)m_idxBuff.Lock();
		int baseIndex = 0;
		for( int i=0; i < rowCellCount; ++i )
		{
			for( int k=0; k < colCellCount; ++k )
			{
				indices[ baseIndex] = (i * colVtxCnt) + k;
				indices[ baseIndex + 1] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 2] = ((i+1) * colVtxCnt) + k;

				indices[ baseIndex + 3] = ((i+1) * colVtxCnt) + k;
				indices[ baseIndex + 4] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 5] = ((i+1) * colVtxCnt) + k + 1;

				// next quad
				baseIndex += 6;
			}
		}	
		m_idxBuff.Unlock();
	}

}


void cGrid2::Render(const int stage)
{
	m_mtrl.Bind();
	m_tex.Bind(stage);
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());
}


void cGrid2::RenderShader(cShader &shader)
{
	shader.Begin();
	shader.BeginPass();

	m_mtrl.Bind(shader);
	m_tex.Bind(shader, "Tex");

	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());

	shader.EndPass();
	shader.End();
}


// 법선 벡터를 다시 계산한다.
void cGrid2::CalculateNormals()
{
	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	WORD *indices = (WORD*)m_idxBuff.Lock();
	for (int i=0; i < m_idxBuff.GetFaceCount()*3; i+=3)
	{
		sVertexNormTex &p1 = vertices[ indices[ i]];
		sVertexNormTex &p2 = vertices[ indices[ i+1]];
		sVertexNormTex &p3 = vertices[ indices[ i+2]];

		Vector3 v1 = p2.p - p1.p;
		Vector3 v2 = p3.p - p1.p;
		v1.Normalize();
		v2.Normalize();
		Vector3 n = v1.CrossProduct(v2);
		n.Normalize();
		p1.n = n;
		p2.n = n;
		p3.n = n;
	}

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
}


// 광선 orig, dir 에 충돌된 면이 있다면 true 를 리턴하고, 충돌 위치를 out에 
// 저장해서 리턴한다.
bool cGrid2::Pick( const Vector3 &orig, const Vector3 &dir, Vector3 &out )
{
	bool isFirst = true;
	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	WORD *indices = (WORD*)m_idxBuff.Lock();
	RETV(!vertices || !indices, false);

	for (int i=0; i < m_idxBuff.GetFaceCount()*3; i+=3)
	{
		const Vector3 &p1 = vertices[ indices[ i]].p;
		const Vector3 &p2 = vertices[ indices[ i+1]].p;
		const Vector3 &p3 = vertices[ indices[ i+2]].p;

		const Triangle tri(p1, p2, p3);
		const Plane p(p1, p2, p3);

		const float dot = dir.DotProduct( p.N );
		if (dot >= 0)
			continue;

		float t;
		if (tri.Intersect(orig, dir, &t))
		{
			if (isFirst)
			{
				isFirst = false;
				out = orig + dir * t;
			}
			else
			{
				const Vector3 v = orig + dir * t;
				if (orig.LengthRoughly(v) < orig.LengthRoughly(out))
					out = v;
			}
		}
	}
	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();

	return !isFirst;
}
