
#include "stdafx.h"
#include "mesh.h"

using namespace graphic;


cMesh::cMesh(const int id, const sRawMesh &rawMesh) : 
	cNode(id, rawMesh.name)
{
	CreateMaterials(rawMesh);

	m_buffers = cResourceManager::Get()->LoadMeshBuffer(rawMesh.name);
}

cMesh::cMesh(const int id, const sRawBone &rawBone) : 
	cNode(id)
{
	//CreateMesh(rawBone.vertices, rawBone.normals, rawBone.tex, rawBone.indices);
}

cMesh::~cMesh()
{
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


// Animation
bool cMesh::Move(const float elapseTime)
{
	return true;
}


// Render
void cMesh::Render(const Matrix44 &parentTm)
{
	RET(!IsRender());
	RET(!m_buffers);

	//if (m_shader)
	//{
	//	RenderShader(*m_shader, parentTm);
	//	return;
	//}


	if (m_buffers->GetAttributes().empty())
	{
		if (!m_mtrls.empty())
			m_mtrls[ 0].Bind();
		if (!m_textures.empty())
			m_textures[ 0]->Bind(0);

		m_buffers->Bind();

		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&tm );
		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_buffers->GetVertexBuffer().GetVertexCount(), 0, 
			m_buffers->GetIndexBuffer().GetFaceCount());
	}
	else
	{
		m_buffers->Bind();

		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&tm );

		for (u_int i=0; i < m_buffers->GetAttributes().size(); ++i)
		{
			const int mtrlId = m_buffers->GetAttributes()[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;
			
			m_mtrls[ mtrlId].Bind();
			if (m_textures[ mtrlId])
				m_textures[ mtrlId]->Bind(0);

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_buffers->GetVertexBuffer().GetVertexCount(), 
				m_buffers->GetAttributes()[ i].faceStart*3, 
				m_buffers->GetAttributes()[ i].faceCount);
		}
	}

	//RenderBoundingBox(parentTm);
}


// 셰이더를 통해 화면을 그린다.
void cMesh::RenderShader( cShader &shader, const Matrix44 &parentTm )
{
	RET(!IsRender());
	RET(!m_buffers);

	shader.SetMatrix( "mVP", cMainCamera::Get()->GetViewProjectionMatrix());
	shader.SetVector( "vLightDir", Vector3(0,-1,0) );
	shader.SetVector( "vEyePos", cMainCamera::Get()->GetEyePos());

	if (m_buffers->GetAttributes().empty())
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

		m_buffers->Bind();

		shader.Begin();
		shader.BeginPass();

		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_buffers->GetVertexBuffer().GetVertexCount(), 0, 
			m_buffers->GetIndexBuffer().GetFaceCount());

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

		m_buffers->Bind();

		for (u_int i=0; i < m_buffers->GetAttributes().size(); ++i)
		{
			const int mtrlId = m_buffers->GetAttributes()[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;

			m_mtrls[ mtrlId].Bind(shader);
			if (m_textures[ mtrlId])
				m_textures[ mtrlId]->Bind(shader, "Tex");

			shader.BeginPass();

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_buffers->GetVertexBuffer().GetVertexCount(), 
				m_buffers->GetAttributes()[ i].faceStart*3, 
				m_buffers->GetAttributes()[ i].faceCount);

			shader.EndPass();
		}

		shader.End();
	}
}


// 그림자 출력.
void cMesh::RenderShadow(const Matrix44 &viewProj, 
	const Vector3 &lightPos, const Vector3 &lightDir, const Matrix44 &parentTm)
{
	RET(!IsRender());
	RET(!m_shader);
	RET(!m_buffers);

	m_shader->SetMatrix( "mVP", viewProj);
	m_shader->SetVector( "vLightDir", lightDir );
	m_shader->SetVector( "vEyePos", lightPos);

	if (m_buffers->GetAttributes().empty())
	{
		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		m_shader->SetMatrix("mWorld", tm);

		Matrix44 wit = tm.Inverse();
		wit.Transpose();
		m_shader->SetMatrix("mWIT", wit);

		m_buffers->Bind();

		m_shader->Begin();
		m_shader->BeginPass(1);

		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_buffers->GetVertexBuffer().GetVertexCount(), 0, 
			m_buffers->GetIndexBuffer().GetFaceCount());

		m_shader->End();
		m_shader->EndPass();
	}
	else
	{
		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		m_shader->SetMatrix("mWorld", tm);

		Matrix44 wit = tm.Inverse();
		wit.Transpose();
		m_shader->SetMatrix("mWIT", wit);

		m_shader->Begin();

		m_buffers->Bind();

		for (u_int i=0; i < m_buffers->GetAttributes().size(); ++i)
		{
			const int mtrlId = m_buffers->GetAttributes()[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;

			m_shader->BeginPass(1);

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_buffers->GetVertexBuffer().GetVertexCount(), 
				m_buffers->GetAttributes()[ i].faceStart*3, 
				m_buffers->GetAttributes()[ i].faceCount);

			m_shader->EndPass();
		}

		m_shader->End();
	}

}


void cMesh::RenderShadow(cShader &shader, const Matrix44 &parentTm)
{
	RET(!IsRender());
	RET(!m_buffers);

	if (m_buffers->GetAttributes().empty())
	{
		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		shader.SetMatrix("mWorld", tm);

		Matrix44 wit = tm.Inverse();
		wit.Transpose();
		shader.SetMatrix("mWIT", wit);

		//const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		//shader.SetMatrix("mWorld", tm);
		//Matrix44 wit = tm.Inverse();
		//wit.Transpose();
		//shader.SetMatrix("mWIT", wit);
		//if (!m_mtrls.empty())
		//	m_mtrls[ 0].Bind(shader);
		//if (!m_textures.empty())
		//	m_textures[ 0]->Bind(shader, "Tex");

		m_buffers->Bind();

		shader.Begin();
		shader.BeginPass();

		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_buffers->GetVertexBuffer().GetVertexCount(), 0, 
			m_buffers->GetIndexBuffer().GetFaceCount());

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

		//const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		//shader.SetMatrix("mWorld", tm);
		//Matrix44 wit = tm.Inverse();
		//wit.Transpose();
		//shader.SetMatrix("mWIT", wit);

		shader.Begin();
		m_buffers->Bind();
		//m_vtxBuff.Bind();
		//m_idxBuff.Bind();

		for (u_int i=0; i < m_buffers->GetAttributes().size(); ++i)
		{
			const int mtrlId = m_buffers->GetAttributes()[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;

			//m_mtrls[ mtrlId].Bind(shader);
			//if (m_textures[ mtrlId])
			//	m_textures[ mtrlId]->Bind(shader, "Tex");

			shader.BeginPass();

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_buffers->GetVertexBuffer().GetVertexCount(), 
				m_buffers->GetAttributes()[ i].faceStart*3, 
				m_buffers->GetAttributes()[ i].faceCount);

			shader.EndPass();
		}

		shader.End();
	}
}


// Render Bounding Box
void cMesh::RenderBoundingBox(const Matrix44 &tm)
{
	RET(!IsRender());

	m_boundingBox.Render(m_localTM * m_aniTM * m_TM * tm);
}


// 경계박스 생성.
void cMesh::CreateBoundingBox(OUT cCube &out)
{
	sMinMax mm;

	sVertexNormTexSkin* pv = (sVertexNormTexSkin*)m_buffers->GetVertexBuffer().Lock();
	for (int i = 0; i < m_buffers->GetVertexBuffer().GetVertexCount(); i++)
	{
		const Vector3 pos = pv[ i].p;
		mm.Update(pos);
	}
	m_buffers->GetVertexBuffer().Unlock();

	out.SetCube(mm.Min, mm.Max);
}
