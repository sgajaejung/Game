
#include "stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel(id)
,	m_weapon(NULL)
,	m_weaponNode1(NULL)
{

}

cCharacter::~cCharacter()
{
	SAFE_DELETE(m_weapon);
}


bool cCharacter::Create(const string &modelName)
{
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
		m_weapon = new cModel(100);

	if (!m_weapon->Create(fileName))
		return;

	m_weaponBoneNode1 = m_weapon->GetBoneMgr()->FindBone("Mace_Hand_Dummy");
	m_weaponBoneNode2 = m_weapon->GetBoneMgr()->FindBone("Shield_Rotate_Dummy");
}


bool cCharacter::Move(const float elapseTime)
{
	cModel::Move(elapseTime);
	
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


void cCharacter::Render()
{
	cModel::Render();

	if (m_weapon)
		m_weapon->Render();
}


void cCharacter::RenderShader(cShader &shader)
{
	cModel::RenderShader(shader);

	if (m_weapon)
		m_weapon->RenderShader(shader);
}


void cCharacter::RenderShadow(cShader &shader)
{
	cModel::RenderShadow(shader);

	if (m_weapon)
		m_weapon->RenderShadow(shader);
}


void cCharacter::SetRenderWeaponBoundingBox(const bool isRenderBoundingBox)
{
	if (m_weapon)
		m_weapon->SetRenderBoundingBox(isRenderBoundingBox);
}


void cCharacter::SetTM(const Matrix44 &tm)
{
	cModel::SetTM(tm);
	if (m_weapon)
		m_weapon->SetTM(tm);
}


void cCharacter::MultiplyTM(const Matrix44 &tm)
{
	cModel::MultiplyTM(tm);
	if (m_weapon)
		m_weapon->MultiplyTM(tm);
}

