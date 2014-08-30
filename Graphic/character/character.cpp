
#include "stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel(id)
,	m_action(CHARACTER_ACTION::NONE)
{

}

cCharacter::~cCharacter()
{

}


// 캐릭터에게 명령을 내린다.
void cCharacter::Action(const CHARACTER_ACTION::TYPE type, const int param1, const int param2)
	// param1=0, param2 = 0
{
	RET(m_action == type);

	BOOST_FOREACH (auto &action, m_actions)
	{
		if (action.type == type)
		{
			m_action = type;
			SetAnimation(action.animationFile);
			break;
		}
	}

}


// 
void cCharacter::SetActionData(const vector<sActionData> &actions)
{
	m_actions = actions;
}
