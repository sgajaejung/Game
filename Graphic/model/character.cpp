
#include "stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel(id)
,	m_weapon(NULL)
,	m_weaponNode1(NULL)
,	m_weaponBoneNode1(NULL)
,	m_weaponBoneNode2(NULL)
{

}

cCharacter::~cCharacter()
{
	SAFE_DELETE(m_weapon);
}


bool cCharacter::Create(const string &modelName)
{
	SetShader( 
		cResourceManager::Get()->LoadShader("hlsl_skinning_no_light.fx") );

	m_weaponNode1 = NULL;
	return cModel::Create(modelName);
}


void cCharacter::LoadWeapon(const string &fileName)
{
	SAFE_DELETE(m_weapon);

	RET(!m_bone);
	//m_weaponNode = m_bone->FindBone("dummy_weapon");
	//m_weaponNode = m_bone->FindBone("Handle02");
	m_weaponNode1 = m_bone->FindBone("Mace_Hand_Dummy");
	m_weaponNode2= m_bone->FindBone("Shield_Rotate_Dummy");
	RET(!m_weaponNode1);
	RET(!m_weaponNode2);

	if (!m_weapon)
		m_weapon = new cModel(common::GenerateId());

	if (!m_weapon->Create(fileName))
		return;

	m_weaponBoneNode1 = m_weapon->GetBoneMgr()->FindBone("Mace_Hand_Dummy");
	m_weaponBoneNode2 = m_weapon->GetBoneMgr()->FindBone("Shield_Rotate_Dummy");
}


bool cCharacter::Move(const float elapseTime)
{
	cModel::Move(elapseTime);

	if (m_weapon)
		m_weapon->SetTM(m_TM);
	
	if (m_weapon && m_weaponNode1 && m_weaponBoneNode1)
	{
		const Matrix44 mat = m_bone->GetPalette()[ m_weaponNode1->GetId()];
		m_weapon->GetBoneMgr()->GetPalette()[ m_weaponBoneNode1->GetId()] = mat;
		m_weaponBoneNode1->SetAccTM( m_weaponNode1->GetAccTM() );
	}

	if (m_weapon && m_weaponNode2 && m_weaponBoneNode2)
	{
		const Matrix44 mat = m_bone->GetPalette()[ m_weaponNode2->GetId()];
		m_weapon->GetBoneMgr()->GetPalette()[ m_weaponBoneNode2->GetId()] = mat;
		m_weaponBoneNode2->SetAccTM( m_weaponNode2->GetAccTM() );
	}

	return true;
}


void cCharacter::Render(const Matrix44 &tm)
{
	cModel::Render(tm);

	if (m_weapon)
		m_weapon->Render(tm);
}


void cCharacter::SetRenderWeaponBoundingBox(const bool isRenderBoundingBox)
{
	if (m_weapon)
		m_weapon->SetRenderBoundingBox(isRenderBoundingBox);
}


// 캐릭터에게 명령을 내린다.
void cCharacter::Action(const CHARACTER_ACTION::TYPE type, const int param1, const int param2)
	// param1=0, param2 = 0
{
	BOOST_FOREACH (auto &action, m_actions)
	{
		if (action.type == type)
		{
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
