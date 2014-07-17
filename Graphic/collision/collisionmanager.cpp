
#include "stdafx.h"
#include "collisionmanager.h"
#include "collision.h"
#include <list>


using namespace graphic;


cCollisionManager::cCollisionManager()
{
	m_group1 = new sCollisionNode( 0 );
	m_group2 = new sCollisionNode( 0 );
}

cCollisionManager::~cCollisionManager()
{
	SAFE_DELETE(m_group1);
	SAFE_DELETE(m_group2);
}


//-----------------------------------------------------------------------------//
// nGroup: 0, 1 만 지원한다.
// 충돌테스트는 다른 그룹끼리만 가능하다.
// pObj : 충돌테스트할 오브젝트
// nTestNum : 충돌테스트 번호이며, 다른 그룹에서 같은 충돌테스트번호 끼리 충돌테스트한다.
//-----------------------------------------------------------------------------//
void cCollisionManager::InsertObject( int group, ICollisionable *obj, int nTestNum )
{
	sCollisionNode *node;
	if (0 == group) 
		node = m_group1;
	else if (1 == group) 
		node = m_group2;
	else 
		return;

	InsertObject( node, obj, nTestNum );
}


//-----------------------------------------------------------------------------//
// 충돌테스트할 오브젝트 추가
// pParent : 부모 충돌 오브젝트
// pParent 오브젝트가 충돌된후 자식으로 pObj 오브젝트가 충돌테스트 검사된다.
//-----------------------------------------------------------------------------//
void cCollisionManager::InsertObject( ICollisionable *parent, ICollisionable *obj, int testNum )
{
	RET(!parent);

	// 두그룹에서 부모 노드를 찾는다.
	sCollisionNode *node;
	node = (sCollisionNode*)m_group1->FindNode( parent->GetId() );
	if (!node) 
		node = (sCollisionNode*)m_group2->FindNode( parent->GetId() );

	InsertObject( node, obj, testNum );
}


//-----------------------------------------------------------------------------//
// 충돌테스트할 오브젝트 추가
// nTestNum 이 3는 오브젝트가 이동할때 충돌을 체크하는 번호이다.
//-----------------------------------------------------------------------------//
void cCollisionManager::InsertObject( sCollisionNode *parent, ICollisionable *obj, int testNum )
{
	sCollisionNode *pnew = new sCollisionNode( obj->GetId() );
	pnew->testnum = testNum;
	pnew->pobj = obj;
	pnew->box = obj->GetCollisionBox();
	//pnew->psphere = pObj->GetSphere();

	parent->InsertChild(pnew);
}


//-----------------------------------------------------------------------------//
// 리스트에서 오브젝트 추가
//-----------------------------------------------------------------------------//
void cCollisionManager::RemoveObject( ICollisionable *obj )
{
	m_group1->RemoveNode( obj->GetId() );
	m_group2->RemoveNode( obj->GetId() );
}


//-----------------------------------------------------------------------------//
// 충돌테스트하기 전에 CollisionBox 좌표를 업데이트한다.
// 모델이 이동되었다면 CollisionTest() 함수 호출 전에 이 함수를 먼저 호출해야 한다.
//-----------------------------------------------------------------------------//
void cCollisionManager::UpdateCollisionBox()
{
	UpdateCollisionBoxRec(m_group1);
	UpdateCollisionBoxRec(m_group2);
}

void cCollisionManager::UpdateCollisionBoxRec( sCollisionNode *obj )
{
	RET(!obj);

	obj->pobj->UpdateCollisionBox();
	BOOST_FOREACH (auto &node, obj->GetChildren())
		UpdateCollisionBoxRec((sCollisionNode*)node);
}


//-----------------------------------------------------------------------------//
// nTestNum 값으로 설정된 CollisionBox들만 충돌테스트한다.
// 같은그룹끼리는 충돌테스트 하지 않는다.
// 충돌된 오브젝트 갯수를 리턴한다.
//-----------------------------------------------------------------------------//
int cCollisionManager::CollisionTest( int nTestNum )
{
	int cnt = 0;
	BOOST_FOREACH (cNode *p1, m_group1->GetChildren())
	{
		sCollisionNode *node1 = (sCollisionNode*)p1;
		if (!node1->pobj->IsTest(nTestNum))
			continue;

		BOOST_FOREACH (cNode *p2, m_group2->GetChildren())
		{
			sCollisionNode *node2 = (sCollisionNode*)p2;
			if (!node2->pobj->IsTest(nTestNum))
				continue;

			// 충돌 테스트
			if (CheckNodeCollision(node1, node2, nTestNum))
			{
				// 자식이 없다면 충돌테스트 끝
				// nTestNum이 3이라면 루트만 충돌테스트 한다.
//				if( 3 == nTestNum || (!node1->m_pChild && !node2->m_pChild) )
				//{
				//	m_ChkTable[ cnt][ 0] = node1->pobj;
				//	m_ChkTable[ cnt][ 1] = node2->pobj;
				//	++cnt;
				//}
				//else
				{
					//// 자식이 있다면 자식까지 충돌테스트 성공해야 최종적으로 충돌된 상태가 된다.
					//SObjTree *p1 = (SObjTree*)((node1->m_pChild)? node1->m_pChild : node1);
					//SObjTree *p2 = (SObjTree*)((node2->m_pChild)? node2->m_pChild : node2);
					//if (CollisionTest_SrcRec(p1, p2, nTestNum))
					//{
					//	m_ChkTable[ cnt][ 0] = node1->pobj;
					//	m_ChkTable[ cnt][ 1] = node2->pobj;
					//	++cnt;
					//}
				}
			}

		}

	}

	return 0;
}


bool cCollisionManager::CheckNodeCollision(sCollisionNode *node1, sCollisionNode *node2, int testNum )
{
	RETV(!node1 || !node2, false);

	// Src TestNum값이 0일때는 Target의 TestNum상관없이 충돌테스트를 한다.
	// Src TestNum값이 설정되어 있을때는 인자로 넘어온 nTestNum값과 Target의 TestNum이 같을때만 충돌테스트한다.
	if ( ((0 != node1->testnum) && (testNum != node1->testnum)) 
		|| (((0 != node1->testnum) && (0 != node2->testnum)) && (node1->testnum != node2->testnum)) )
		return false;

	// Sphere vs Sphere 충돌테스트

	// Box vs Box 충돌테스트
	if (node1->box && node2->box)
	{
		if (node1->box->Collision(*node2->box))
		{
			//// 자식이 있다면 자식까지 충돌테스트 성공해야 최종적으로 충돌된 상태가 된다.
			sCollisionNode *p1 = (sCollisionNode*)((node1->GetChildren().empty())? node1->m_pChild : node1);
			sCollisionNode *p2 = (sCollisionNode*)((node2->m_pChild)? node2->m_pChild : node2);

			CollisionTestRec(node1, node2, testNum);
		}
	}

	return false;
}


int cCollisionManager::CollisionTestRec( sCollisionNode *srcNode, sCollisionNode *targetNode, int testNum )
{
	RETV(!srcNode || !targetNode, 0);

	if (srcNode->GetChildren().empty() && targetNode->GetChildren().empty())
		return 1;

	int cnt = 0;
	BOOST_FOREACH (cNode *p1, srcNode->GetChildren())
	{
		sCollisionNode *node1 = (sCollisionNode*)p1;
		if (!node1->pobj->IsTest(nTestNum))
			continue;

		BOOST_FOREACH (cNode *p2, targetNode->GetChildren())
		{
			sCollisionNode *node2 = (sCollisionNode*)p2;
			if (!node2->pobj->IsTest(nTestNum))
				continue;

			// 충돌 테스트
			if (CheckNodeCollision(node1, node2, nTestNum))
			{
				// 자식이 없다면 충돌테스트 끝
				// nTestNum이 3이라면 루트만 충돌테스트 한다.
				//				if( 3 == nTestNum || (!node1->m_pChild && !node2->m_pChild) )
				//{
				//	m_ChkTable[ cnt][ 0] = node1->pobj;
				//	m_ChkTable[ cnt][ 1] = node2->pobj;
				//	++cnt;
				//}
				//else
				{
					//// 자식이 있다면 자식까지 충돌테스트 성공해야 최종적으로 충돌된 상태가 된다.
					//SObjTree *p1 = (SObjTree*)((node1->m_pChild)? node1->m_pChild : node1);
					//SObjTree *p2 = (SObjTree*)((node2->m_pChild)? node2->m_pChild : node2);
					//if (CollisionTest_SrcRec(p1, p2, nTestNum))
					//{
					//	m_ChkTable[ cnt][ 0] = node1->pobj;
					//	m_ChkTable[ cnt][ 1] = node2->pobj;
					//	++cnt;
					//}
				}
			}
		}
	}

	return cnt;
}
