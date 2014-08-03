
#include "stdafx.h"
#include "mesh.h"

using namespace graphic;


cMesh::cMesh(const int id, const sRawMesh &rawMesh) : 
	cNode(id)
{
	CreateMesh(rawMesh.vertices, rawMesh.normals, rawMesh.tex, rawMesh.indices);
	CreateBoneWeight(rawMesh.weights);
	CreateMaterials(rawMesh);
	CreateAttributes(rawMesh);
}

cMesh::cMesh(const int id, const sRawBone &rawBone) : 
	cNode(id)
{
	CreateMesh(rawBone.vertices, rawBone.normals, rawBone.tex, rawBone.indices);
}

cMesh::~cMesh()
{

}


void cMesh::CreateMesh( const vector<Vector3> &vertices, 
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
void cMesh::CreateBoneWeight( const vector<sVertexWeight> &weights )
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


// 매터리얼 생성.
void cMesh::CreateMaterials(const sRawMesh &rawMesh)
{
	m_textures.reserve(rawMesh.mtrls.size());
	m_mtrls.reserve(rawMesh.mtrls.size());

	BOOST_FOREACH (auto &mtrl, rawMesh.mtrls)
	{
		m_mtrls.push_back(cMaterial());
		m_mtrls.back().Init(mtrl);

		m_textures.push_back( cResourceManager::Get()->LoadTexture(mtrl.directoryPath, mtrl.texture) );
	}
}


// 속성버퍼 초기화.
void cMesh::CreateAttributes(const sRawMesh &rawMesh)
{
	m_attributes = rawMesh.attributes;

}


// Animation
bool cMesh::Move(const float elapseTime)
{
	return true;
}


// Render
void cMesh::Render(const Matrix44 &parentTm)
{
	if (m_attributes.empty())
	{
		if (!m_mtrls.empty())
			m_mtrls[ 0].Bind();
		if (!m_textures.empty())
			m_textures[ 0]->Bind(0);

		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&tm );
		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());
	}
	else
	{
		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&tm );

		for (u_int i=0; i < m_attributes.size(); ++i)
		{
			const int mtrlId = m_attributes[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;
			
			m_mtrls[ mtrlId].Bind();
			if (m_textures[ mtrlId])
				m_textures[ mtrlId]->Bind(0);

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_vtxBuff.GetVertexCount(), 
				m_attributes[ i].faceStart*3, m_attributes[ i].faceCount);
		}
	}

	//RenderBoundingBox(parentTm);
}


// 기본 셰이더를 통해 출력한다.
void cMesh::RenderShader( const Matrix44 &parentTm )
{
	// 아무일도 없음.
}


// 셰이더를 통해 화면을 그린다.
void cMesh::RenderShader( cShader &shader, const Matrix44 &parentTm )
{
	if (m_attributes.empty())
	{
		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		shader.SetMatrix("mWorld", tm);
		
		Matrix44 wit = tm.Inverse();
		wit.Transpose();
		shader.SetMatrix("mWIT", wit);

		if (!m_mtrls.empty())
			m_mtrls[ 0].Bind(shader);
		if (!m_textures.empty())
			m_textures[ 0]->Bind(shader, "Tex");

		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		shader.Begin();
		shader.BeginPass();

		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());

		shader.End();
		shader.EndPass();
	}
	else
	{
		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		shader.SetMatrix("mWorld", tm);

		Matrix44 wit = tm.Inverse();
		wit.Transpose();
		shader.SetMatrix("mWIT", wit);

		shader.Begin();

		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		for (u_int i=0; i < m_attributes.size(); ++i)
		{
			const int mtrlId = m_attributes[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;

			m_mtrls[ mtrlId].Bind(shader);
			if (m_textures[ mtrlId])
				m_textures[ mtrlId]->Bind(shader, "Tex");

			shader.BeginPass();

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_vtxBuff.GetVertexCount(), 
				m_attributes[ i].faceStart*3, m_attributes[ i].faceCount);

			shader.EndPass();
		}

		shader.End();
	}
}


void cMesh::RenderShadow(cShader &shader, const Matrix44 &parentTm)
{
	if (m_attributes.empty())
	{
		//const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		//shader.SetMatrix("mWorld", tm);
		//Matrix44 wit = tm.Inverse();
		//wit.Transpose();
		//shader.SetMatrix("mWIT", wit);
		//if (!m_mtrls.empty())
		//	m_mtrls[ 0].Bind(shader);
		//if (!m_textures.empty())
		//	m_textures[ 0]->Bind(shader, "Tex");

		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		shader.Begin();
		shader.BeginPass();

		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());

		shader.End();
		shader.EndPass();
	}
	else
	{
		//const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		//shader.SetMatrix("mWorld", tm);
		//Matrix44 wit = tm.Inverse();
		//wit.Transpose();
		//shader.SetMatrix("mWIT", wit);

		shader.Begin();
		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		for (u_int i=0; i < m_attributes.size(); ++i)
		{
			const int mtrlId = m_attributes[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;

			//m_mtrls[ mtrlId].Bind(shader);
			//if (m_textures[ mtrlId])
			//	m_textures[ mtrlId]->Bind(shader, "Tex");

			shader.BeginPass();

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_vtxBuff.GetVertexCount(), 
				m_attributes[ i].faceStart*3, m_attributes[ i].faceCount);

			shader.EndPass();
		}

		shader.End();
	}
}


// Render Bounding Box
void cMesh::RenderBoundingBox(const Matrix44 &tm)
{
	m_boundingBox.Render(m_localTM * m_aniTM * m_TM * tm);
}


// 경계박스 생성.
void cMesh::CreateBoundingBox(OUT cCube &out)
{
	sMinMax mm;

	sVertexNormTexSkin* pv = (sVertexNormTexSkin*)m_vtxBuff.Lock();
	for (int i = 0; i < m_vtxBuff.GetVertexCount(); i++)
	{
		const Vector3 pos = pv[ i].p;
		mm.Update(pos);
	}
	m_vtxBuff.Unlock();

	out.SetCube(mm.Min, mm.Max);
}
