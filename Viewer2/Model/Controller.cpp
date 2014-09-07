
#include "stdafx.h"
#include "Controller.h"

using namespace graphic;


cController::cController() : 
	m_analyzer(NULL)
,	m_isPlay(true)
,	m_archeCharacter(common::GenerateId())
,	m_viewerDlg(NULL)
,	m_modelName("modelName: ", 10, 27)
,	m_animationName("animationName: ", 10, 44)
{
	m_analyzer = new cCharacterAnalyzer();
	m_analyzer->SetCharacter(&m_archeCharacter);
}

cController::~cController()
{
	SAFE_DELETE(m_analyzer);
}


bool cController::LoadFile( const string &fileName )
{
	cCharacter *character = GetCharacter();
	RETV(!character, false);

	ShowLoadingDialog();

	bool result = false;
	const RESOURCE_TYPE::TYPE type = cResourceManager::Get()->GetFileKind(fileName);
	switch (type)
	{
	case RESOURCE_TYPE::MESH:
		m_currentMeshFileName = fileName;
		if (result = character->Create(fileName))
		{
			m_modelName.SetText("model: " + common::GetFileName(fileName));
			m_animationName.SetText("");
		}
		break;

	case RESOURCE_TYPE::ANIMATION:
		m_currentAnimationFileName = fileName;
		if (result = character->SetAnimation(fileName))
			m_animationName.SetText("animation: " + common::GetFileName(fileName));
		break;

	default:
		break;
	}

	NotifyObserver();

	HideLoadingDialog();
	return result;
}


void cController::Render()
{
	RET(!m_analyzer);
	m_analyzer->Render(Matrix44::Identity);
	m_modelName.Render();
	m_animationName.Render();
}


//void cController::RenderShader(graphic::cShader &shader)
//{
//	RET(!m_character);
//	m_character->RenderShader(shader);
//}


void cController::Update(const float elapseT)
{
	RET(!m_analyzer);

	if (m_isPlay)
		m_analyzer->Move(elapseT);
	else
		m_analyzer->Move(0);
}


void cController::SetCurrentAnimationFrame(const int curFrame)
{
	cCharacter *character = GetCharacter();
	RET(!character);

	cBoneMgr *boneMgr = character->GetBoneMgr();
	RET(!boneMgr);
	boneMgr->SetCurrentAnimationFrame(curFrame);
}


// 패널이 전환 될 때 호출된다.
void cController::ChangePanel(const int panelIdx)
{
	switch (panelIdx)
	{
	case 0: // file panel
		m_analyzer->SetCharacter(&m_archeCharacter);
		break;

	case 1: // model panel
		break;

	case 2: // animation panel
		break;

	case 3: // archeblade panel
		m_analyzer->SetCharacter(&m_archeCharacter);
		break;

	case 4: // tera panel
		m_analyzer->SetCharacter(&m_teraCharacter);
		break;
	}

}


// observer 에게 업데이트 메세지를 보낸다.
void cController::SendUpdate(const int type) //type=0
{
	NotifyObserver();
}
