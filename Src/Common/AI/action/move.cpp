
#include "stdafx.h"
#include "move.h"
#include "../object/aiinterface.h"

using namespace ai;


cMove::cMove() 
	: cAction(NULL, "move", "zealot_walk.ani", ACTION_TYPE::MOVE) 
{

}


void cMove::Init(IArtificialIntelligence *obj, const Vector3 &dest)
{
	SetOwner(obj);
	m_dest = dest;
}


void cMove::Start(cAction *prevAction)
{
	cAction::Start(prevAction);

	IArtificialIntelligence *obj = GetOwner();
	RET(!obj);

	const Vector3 curPos = obj->aiGetTransform().GetPosition();
	m_dir = m_dest - curPos;
	m_dir.Normalize();

	Quaternion q;
	q.SetRotationArc(Vector3(0,0,-1), m_dir);
	obj->aiGetTransform() = q.GetMatrix();
	obj->aiGetTransform().SetPosition(curPos);

	obj->aiSetAnimation( "zealot_walk.ani" );
}


bool cMove::ActionUpdate(const float elapseTime)
{
	IArtificialIntelligence *obj = GetOwner();
	RETV(!obj, false);

	Vector3 pos = obj->aiGetTransform().GetPosition() + m_dir * elapseTime;
	obj->aiGetTransform().SetPosition(pos);

	if (pos.LengthRoughly(m_dest) < 0.01f)
		return false; // ¸ñÀûÁö µµÂø.

	return true;
}

