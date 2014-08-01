
#include "stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel(id)
,	m_weapon(NULL)
,	m_weaponNode(NULL)
{

}

cCharacter::~cCharacter()
{
	SAFE_DELETE(m_weapon);
}


bool cCharacter::Create(const string &modelName)
{
	m_weaponNode = NULL;
	return cModel::Create(modelName);
}


void cCharacter::LoadWeapon(const string &fileName)
{
	SAFE_DELETE(m_weapon);

	RET(!m_bone);
	//m_weaponNode = m_bone->FindBone("dummy_weapon");
	m_weaponNode = m_bone->FindBone("Handle02");
	//m_weaponNode = m_bone->FindBone("Bip01-L-Finger2");
	RET(!m_weaponNode);

	if (!m_weapon)
		m_weapon = new cModel(100);

	if (!m_weapon->Create(fileName, MODEL_TYPE::RIGID))
		return;
}


bool cCharacter::Move(const float elapseTime)
{
	cModel::Move(elapseTime);
	
	if (m_weapon && m_weaponNode)
	{
		//const Matrix44 mat = m_weaponNode->GetAccTM();
		//m_weapon->SetTM(mat * m_matTM);
		//m_weapon->Move(elapseTime);

		//const Matrix44 mat = m_bone->GetPalette()[ m_weaponNode->GetId()];
		const Matrix44 mat = m_weaponNode->GetAccTM();
		//const Matrix44 mat = m_weaponNode->GetOffset();
		m_weapon->SetTM(mat * m_matTM);
		m_weapon->Move(elapseTime);
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
