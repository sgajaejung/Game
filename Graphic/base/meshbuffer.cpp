
#include "stdafx.h"
#include "meshbuffer.h"


using namespace graphic;

cMeshBuffer::cMeshBuffer()
{
}

cMeshBuffer::cMeshBuffer(const sRawMesh &rawMesh)
{
	CreateMesh(rawMesh.vertices, rawMesh.normals, rawMesh.tex, rawMesh.indices);
	CreateBoneWeight(rawMesh.weights);
	CreateAttributes(rawMesh);
}

cMeshBuffer::cMeshBuffer(const sRawBone &rawBone)
{
	CreateMesh(rawBone.vertices, rawBone.normals, rawBone.tex, rawBone.indices);
}


void cMeshBuffer::Bind()
{
	m_idxBuff.Bind();
	m_vtxBuff.Bind();
}


void cMeshBuffer::CreateMesh( const vector<Vector3> &vertices, 
	const vector<Vector3> &normals, 
	const vector<Vector3> &tex,
	const vector<int> &indices )
{
	const bool isTexture = !tex.empty();

	// 버텍스 버퍼 생성.
	if (m_vtxBuff.Create(vertices.size(), sizeof(sVertexNormTexSkin), sVertexNormTexSkin::FVF))
	{
		sVertexNormTexSkin* pv = (sVertexNormTexSkin*)m_vtxBuff.Lock();
		for (u_int i = 0; i < vertices.size(); i++)
		{
			pv[ i].p = vertices[ i];
			pv[ i].n = normals[ i];
			if (isTexture)
			{
				pv[ i].u = tex[ i].x;
				pv[ i].v = tex[ i].y;
			}
		}
		m_vtxBuff.Unlock();
	}

	// 인덱스 버퍼 생성.
	if (m_idxBuff.Create(indices.size()))
	{
		WORD *pi = (WORD*)m_idxBuff.Lock();
		for (u_int i = 0; i < indices.size(); ++i)
			pi[ i] = indices[ i];
		m_idxBuff.Unlock();
	}

	//CreateBoundingBox(m_boundingBox);
}


// 본 인덱스, 가중치를 설정한다.s
void cMeshBuffer::CreateBoneWeight( const vector<sVertexWeight> &weights )
{

	if (sVertexNormTexSkin* pv = (sVertexNormTexSkin*)m_vtxBuff.Lock())
	{
		for (u_int i=0; i <weights.size(); ++i)
		{
			const sVertexWeight &weight = weights[ i];
			const int vtxIdx = weight.vtxIdx;

			ZeroMemory(pv[ vtxIdx].weights, sizeof(float)*4);
			ZeroMemory(pv[ vtxIdx].matrixIndices, sizeof(float)*4);
			//pv[ vtxIdx].matrixIndices = 0;

			for (int k=0; (k < weight.size) && (k < 4); ++k)
			{
				const sWeight *w = &weight.w[ k];
				if (k < 3)
				{
					pv[ vtxIdx].weights[ k] = w->weight;
				}
				else // k == 3 (마지막 가중치)
				{
					pv[ vtxIdx].weights[ k] = 
						1.f - (pv[ vtxIdx].weights[ 0] + pv[ vtxIdx].weights[ 1] + pv[ vtxIdx].weights[ 2]);
				}

				pv[ vtxIdx].matrixIndices[ k] = w->bone;
				//const int boneIdx = (w->bone << (8*(3-k)));
				//pv[ vtxIdx].matrixIndices |= boneIdx;
			}
		}

		m_vtxBuff.Unlock();
	}

}


// 속성버퍼 초기화.
void cMeshBuffer::CreateAttributes(const sRawMesh &rawMesh)
{
	m_attributes = rawMesh.attributes;
}


// 인스턴스를 깊이복사해서 리턴한다.
cMeshBuffer* cMeshBuffer::Clone()
{
	cMeshBuffer *mesh = new cMeshBuffer();
	mesh->m_attributes = m_attributes;
	mesh->m_vtxBuff = m_vtxBuff;
	mesh->m_idxBuff = m_idxBuff;
	return mesh;
}
