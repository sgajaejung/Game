
#include "stdafx.h"
#include "rigidmesh.h"

using namespace graphic;


cRigidMesh::cRigidMesh(const int id, const sRawMesh &raw) : 
	cMesh(id, raw)
,	m_track(NULL)
,	m_aniTime(0)
,	m_aniStart(0)
,	m_aniEnd(0)
,	m_aniFrame(0)
{
	CreateBoundingBox(m_boundingBox);

}

cRigidMesh::~cRigidMesh()
{
	SAFE_DELETE(m_track);
}


// 애니메이션 로딩.
void cRigidMesh::LoadAnimation( const sRawAni &rawAni )
{
	SAFE_DELETE(m_track);

	m_track = new cTrack(rawAni);
	m_aniStart = (int)rawAni.start;
	m_aniEnd = (int)rawAni.end;
	m_aniFrame = (int)rawAni.start;
}


// 애니메이션 처리
bool cRigidMesh::Move(const float elapseTime)
{
	RETV(!m_track, false);

	//  프레임 단위로 변환한다.
	m_aniTime += elapseTime;
	m_aniFrame = (int)(m_aniTime * 30.f);

	if (m_aniFrame > m_aniEnd)
	{
		m_aniTime = m_aniStart * 30.f;
		m_aniFrame = m_aniStart;
		m_track->InitAnimation();
	}

	m_aniTM.SetIdentity();
	m_track->Move(m_aniFrame, m_aniTM);
	return true;
}


// 출력.
void cRigidMesh::Render(const Matrix44 &parentTm)
{
	cMesh::Render(parentTm);
	RenderBoundingBox(parentTm);
}


// Render Bounding Box
void cRigidMesh::RenderBoundingBox(const Matrix44 &tm)
{
	m_boundingBox.Render(m_localTM * m_aniTM * m_TM * tm);
}


// 경계박스 생성.
void cRigidMesh::CreateBoundingBox(OUT cCube &out)
{
	sMinMax mm;

	sVertexNormTex* pv = (sVertexNormTex*)m_vtxBuff.Lock();
	for (int i = 0; i < m_vtxBuff.GetVertexCount(); i++)
	{
		const Vector3 pos = pv[ i].p;

		if (mm.Min.x > pos.x)
			mm.Min.x = pos.x;
		if (mm.Min.y > pos.y)
			mm.Min.y = pos.y;
		if (mm.Min.z > pos.z)
			mm.Min.z = pos.z;

		if (mm.Max.x < pos.x)
			mm.Max.x = pos.x;
		if (mm.Max.y < pos.y)
			mm.Max.y = pos.y;
		if (mm.Max.z < pos.z)
			mm.Max.z = pos.z;
	}
	m_vtxBuff.Unlock();

	out.SetCube(mm.Min, mm.Max);
}
