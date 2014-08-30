
#include "stdafx.h"
#include "character.h"
#include "characteranalyzer.h"

using namespace graphic;


cCharacterAnalyzer::cCharacterAnalyzer(const int id) :
	cCharacter(id)
,	m_selectBone(NULL)
{

}

cCharacterAnalyzer::~cCharacterAnalyzer()
{
}


// 캐릭터 파일 로드.
bool cCharacterAnalyzer::Create(const string &modelName)
{
	const bool result = cCharacter::Create(modelName);

	m_selectBone = NULL;
	m_selectBoneCube.SetCube(Vector3(0,0,0), Vector3(0,0,0));

	return result;
}


bool cCharacterAnalyzer::Move(const float elapseTime)
{
	cCharacter::Move(elapseTime);

	if (m_selectBone)
		m_selectBoneCube.SetTransform( m_selectBone->GetAccTM() );

	return true;
}


void cCharacterAnalyzer::Render(const Matrix44 &tm)
{
	cCharacter::Render(tm);

	// 선택된 본 경계 박스 출력.
	GetDevice()->SetRenderState(D3DRS_ZENABLE, 0);	
	m_selectBoneCube.Render(m_TM * tm);
	GetDevice()->SetRenderState(D3DRS_ZENABLE, 1);

}


// 본을 강조한다.
// 경계박스로 본을 출력한다.
void cCharacterAnalyzer::HighlightBone(const string &boneName)
{
	RET(!m_bone);

	if (cBoneNode *node = m_bone->FindBone(boneName))
	{
		if ((node->GetId()>=0) && (node->GetId() < (int)m_bone->GetBoundingBoxes().size()))
		{
			m_selectBone = node;
			m_selectBoneCube.SetColor(0x00FF00);
			m_selectBoneCube.SetCube( m_bone->GetBoundingBoxes()[ node->GetId()] );
			if (m_selectBoneCube.Length() < 0.001f)
			{
				const float len = m_boundingBox.Length() / 100.f;
				m_selectBoneCube.SetCube(Vector3(-1,-1,-1)*len, Vector3(1,1,1)*len);
			}
		}
	}
}
