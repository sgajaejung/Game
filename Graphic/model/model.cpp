
#include "stdafx.h"
#include "model.h"
#include "node.h"
#include "../manager/resourcemanager.h"
#include "mesh.h"
#include "bonemgr.h"


using namespace graphic;



cModel::cModel(const int id) :
	m_id(id)
,	m_bone(NULL)
,	m_isRenderMesh(true)
,	m_isRenderBone(false)
,	m_isRenderBoundingBox(false)
,	m_type(MODEL_TYPE::RIGID)
,	m_curAni(NULL)
{
	
}

cModel::~cModel()
{
	Clear();
}


bool cModel::Create(const string &modelName, MODEL_TYPE::TYPE type )
	// type = MODEL_TYPE::AUTO
{
	sRawMeshGroup *rawMeshes = cResourceManager::Get()->LoadModel(modelName);
	RETV(!rawMeshes, false);

	Clear();

	const bool isSkinnedMesh = !rawMeshes->bones.empty();

	// 스키닝 애니메이션이면 Bone을 생성한다.
	switch (type)
	{
	case MODEL_TYPE::RIGID:
		m_type = MODEL_TYPE::RIGID;
		break;

	case MODEL_TYPE::AUTO:
	case MODEL_TYPE::SKIN:
		if (isSkinnedMesh)
		{
			m_bone = new cBoneMgr(0, *rawMeshes);
			m_type = MODEL_TYPE::SKIN;
		}
		else
		{
			m_type = MODEL_TYPE::RIGID;
		}
		break;
	}

	// 메쉬 생성.
	int id = 0;
	BOOST_FOREACH (auto &mesh, rawMeshes->meshes)
	{
		cMesh *p = NULL;
		if (MODEL_TYPE::SKIN == m_type)
		{
			p = new cSkinnedMesh(id++, m_bone->GetPalette(), mesh);
		}
		else
		{
			p = new cRigidMesh(id++, mesh);
		}

		if (p)
			m_meshes.push_back(p);
	}
	
	return true;
}


// 애니메이션 시작.
void cModel::SetAnimation( const string &aniFileName)
{
	if (sRawAniGroup *rawAnies = cResourceManager::Get()->LoadAnimation(aniFileName))
	{
		m_curAni = rawAnies;

		if (MODEL_TYPE::SKIN == m_type)
		{
			m_bone->SetAnimation(*rawAnies, 0);
		}
		else
		{
			for (u_int i=0; i < m_meshes.size(); ++i)
			{
				((cRigidMesh*)m_meshes[ i])->LoadAnimation(rawAnies->anies[0]);
			}
		}
	}
}


bool cModel::Move(const float elapseTime)
{
	BOOST_FOREACH (auto node, m_meshes)
		node->Move(elapseTime);

	if (m_bone)
		m_bone->Move(elapseTime);

	return true;
}


void cModel::Render()
{
	Matrix44 identity;
	GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&identity);

	if (m_isRenderMesh)
	{
		BOOST_FOREACH (auto node, m_meshes)
			node->Render(m_matTM);
	}

	if (m_isRenderBone && m_bone)
		m_bone->Render(m_matTM);

	if (m_bone && m_isRenderBoundingBox)
		m_bone->RenderBoundingBox(m_matTM);
}


void cModel::RenderShader(cShader &shader)
{
	if (m_isRenderMesh)
	{
		BOOST_FOREACH (auto node, m_meshes)
			node->RenderShader(shader, m_matTM);
	}

	// 셰이더 쓰지 않고 그냥 출력.
	if (m_isRenderBone && m_bone)
		m_bone->Render(m_matTM);

	if (m_bone && m_isRenderBoundingBox) 
		m_bone->RenderBoundingBox(m_matTM);
}


void cModel::RenderShadow(cShader &shader)
{
	if (m_isRenderMesh)
	{
		BOOST_FOREACH (auto node, m_meshes)
			node->RenderShadow(shader, m_matTM);
	}
}


// remove all data
void cModel::Clear()
{
	BOOST_FOREACH (auto mesh, m_meshes)
	{
		SAFE_DELETE(mesh);
	}
	m_meshes.clear();
	m_curAni = NULL;

	SAFE_DELETE(m_bone);
}


// 메쉬를 찾아서 리턴한다.
cMesh* cModel::FindMesh(const string &meshName)
{
	BOOST_FOREACH (auto &mesh, m_meshes)
	{
		if (mesh->GetName() == meshName)
			return (cMesh*)mesh;
	}
	return NULL;
}


void cModel::SetRenderMesh(const bool isRenderMesh) 
{ 
	m_isRenderMesh = isRenderMesh; 
}

void cModel::SetRenderBone(const bool isRenderBone) 
{ 
	m_isRenderBone = isRenderBone; 
}

void cModel::SetRenderBoundingBox(const bool isRenderBoundingBox)
{
	m_isRenderBoundingBox = isRenderBoundingBox;
}


int cModel::GetCollisionId() 
{
	return GetId();
}

bool cModel::IsTest( int testNum )
{
	return true;
}


void cModel::UpdateCollisionBox()
{
	m_boundingBox.SetTransform(m_matTM);
}


cBoundingBox* cModel::GetCollisionBox()
{
	sMinMax mm;
	BOOST_FOREACH (auto &mesh, m_meshes)
	{
		const cCube &cube = mesh->GetBoundingBox();
		mm.Update(cube.GetMin());
		mm.Update(cube.GetMax());
	}

	m_boundingBox.SetBoundingBox(mm.Min, mm.Max);
	return &m_boundingBox;
}


void cModel::Collision( int testNum, ICollisionable *obj )
{

}
