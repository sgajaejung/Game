
#include "stdafx.h"
#include "bonemgr.h"
#include "bonenode.h"

using namespace  graphic;


cBoneMgr::cBoneMgr(const int id, const sRawMeshGroup &rawMeshes) :
	m_root(NULL)
,	m_id(id)
{
	m_palette.resize(rawMeshes.bones.size());

	m_bones.resize(rawMeshes.bones.size(), NULL);

	for (u_int i=0; i < rawMeshes.bones.size(); ++i)
	{
		const int id = rawMeshes.bones[ i].id;
		const int parentId = rawMeshes.bones[ i].parentId;
		bool twoRoot = false;
		if (m_root && (parentId < 0))
			twoRoot = true;

		cBoneNode *bone = new cBoneNode(id, m_palette, rawMeshes.bones[ i]);
		SAFE_DELETE(m_bones[ id]);
		m_bones[ id] = bone;

		if (!twoRoot && (-1 >=  parentId)) // root
		{
			m_root = bone;
		}
		else if (twoRoot)
		{
			// nothing~
		}
		else
		{
			m_bones[ parentId]->InsertChild( bone );
		}
	}

	CreateBoundingBox(rawMeshes);
}

cBoneMgr::~cBoneMgr()
{
	Clear();
}


// 에니메이션 설정.
void cBoneMgr::SetAnimation( const sRawAniGroup &rawAnies, int nAniFrame )
{
	SetAnimationRec( m_root, rawAnies, nAniFrame );
}


// 애니메이션 설정.
void cBoneMgr::SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAnies, int nAniFrame )
{
	RET(!node);
	RET(node->GetId() >= (int)rawAnies.anies.size());

	node->SetAnimation( rawAnies.anies[ node->GetId()], nAniFrame, true );
	BOOST_FOREACH (auto p, node->GetChildren())
	{
		SetAnimationRec((cBoneNode*)p, rawAnies, nAniFrame );
	}
}


void cBoneMgr::SetCurrentAnimationFrame(const int curFrame)
{
	BOOST_FOREACH (auto p, m_bones)
	{
		if (p)
			p->SetCurrentFrame(curFrame);
	}
}


// 애니메이션
bool cBoneMgr::Move(const float elapseTime)
{
	RETV(!m_root, false);
	return m_root->Move(elapseTime);
}


// 스켈레톤 출력.
void cBoneMgr::Render(const Matrix44 &parentTm)
{
	RET(!m_root);
	m_root->Render(parentTm);
}


// 경계박스 출력.
void cBoneMgr::RenderBoundingBox(const Matrix44 &parentTm)
{
	const Matrix44 identity;
	for (int i=0; i < (int)m_boundingBox.size(); ++i)
	{
		m_boundingBox[ i].SetTransform( m_bones[ i]->GetAccTM() * parentTm );
		m_boundingBox[ i].Render(identity);
	}
}


// BoneNode 찾아서 리턴.
cBoneNode* cBoneMgr::FindBone(const int id)
{
	RETV(!m_root, NULL);
	return (cBoneNode*)m_root->FindNode(id);
}


cBoneNode* cBoneMgr::FindBone(const string &name)
{
	RETV(!m_root, NULL);
	return (cBoneNode*)m_root->FindNode(name);
}


// 동적으로 할당된 객체 제거.
void cBoneMgr::Clear()
{
	SAFE_DELETE(m_root);
	m_bones.clear();
}


// 경계박스 생성.
void cBoneMgr::CreateBoundingBox(const sRawMeshGroup &rawMeshes)
{
	RET(!m_root);

	const int boneCount = rawMeshes.bones.size();
	vector<sMinMax> boundingBox(boneCount);
	vector<Matrix44> boneInvers(boneCount);
	for (int i=0; i < boneCount; ++i)
	{
		boneInvers[ i] = rawMeshes.bones[ i].worldTm.Inverse();
		boundingBox[ i] = sMinMax();
	}


	// 특정 뼈대들은 경계박스를 하나로 합친다.
	map<int, int> boneIndices;
	SetBoundingBoxIndex(m_root, boneIndices);


	BOOST_FOREACH (const sRawMesh &mesh, rawMeshes.meshes)
	{
		BOOST_FOREACH (const sVertexWeight &weight, mesh.weights)
		{
			const int vtxIdx = weight.vtxIdx;

			for( int k=0; k < weight.size; ++k )
			{
				const sWeight *w = &weight.w[ k];
				const Vector3 pos = mesh.vertices[ vtxIdx] * boneInvers[ w->bone];

				const int boneIdx = boneIndices[ w->bone];

				if (boundingBox[ boneIdx].Min.x > pos.x)
					boundingBox[ boneIdx].Min.x = pos.x;
				if (boundingBox[ boneIdx].Min.y > pos.y)
					boundingBox[ boneIdx].Min.y = pos.y;
				if (boundingBox[ boneIdx].Min.z > pos.z)
					boundingBox[ boneIdx].Min.z = pos.z;

				if (boundingBox[ boneIdx].Max.x < pos.x)
					boundingBox[ boneIdx].Max.x = pos.x;
				if (boundingBox[ boneIdx].Max.y < pos.y)
					boundingBox[ boneIdx].Max.y = pos.y;
				if (boundingBox[ boneIdx].Max.z < pos.z)
					boundingBox[ boneIdx].Max.z = pos.z;
			}
		}
	}


	m_boundingBox.resize(boneCount);
	for (int i=0; i < boneCount; ++i)
	{
		const Vector3 Min = boundingBox[ i].Min;
		const Vector3 Max = boundingBox[ i].Max;

		if (Min.IsEmpty() && Max.IsEmpty())
			continue;

		m_boundingBox[ i].SetCube( Min, Max );
	}
}


// 뼈대 인덱스를 설정한다.
void cBoneMgr::SetBoundingBoxIndex(cBoneNode *node, OUT map<int, int> &boneIndices, const int boneIdx) //boneIdx=-1
{
	RET(!node);

	int nextBoneIdx = -1;
	if (boneIdx == -1)
	{
		// 본 자신의 아이디를 저장한다.
		boneIndices[ node->GetId()] = node->GetId();

		// 특정 뼈대는 하나로 합친다.
		// 손목 아래 뼈대 통합.
		// 발목 아래 뼈대 통합.
		if ((node->GetName() ==  "Bip01 L Hand") 
			||  (node->GetName() ==  "Bip01 R Hand")
			|| (node->GetName() ==  "Bip01 L Foot")
			|| (node->GetName() ==  "Bip01 R Foot")
			)
		{
			nextBoneIdx = node->GetId();
		}
	}
	else
	{
		// 부모의 인덱스로 저장한다.
		boneIndices[ node->GetId()] = boneIdx;
		nextBoneIdx = boneIdx;
	}

	BOOST_FOREACH (auto &child, node->GetChildren())
		SetBoundingBoxIndex((cBoneNode*)child, boneIndices, nextBoneIdx);
}
