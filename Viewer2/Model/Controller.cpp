
#include "stdafx.h"
#include "Controller.h"



cController::cController() : 
	m_model(NULL)
,	m_isPlay(true)
{
	m_model = new graphic::cModel();
}

cController::~cController()
{
	SAFE_DELETE(m_model);
}


bool cController::LoadFile( const string &fileName )
{
	bool result = false;
	const graphic::RESOURCE_TYPE::TYPE type = graphic::cResourceManager::Get()->GetFileKind(fileName);
	switch (type)
	{
	case graphic::RESOURCE_TYPE::MESH:
		m_currentMeshFileName = fileName;
		result = m_model->Create(fileName);
		break;

	case graphic::RESOURCE_TYPE::ANIMATION:
		m_currentAnimationFileName = fileName;
		m_model->SetAnimation(fileName);
		result = true;
		break;

	default:
		break;
	}

	NotifyObserver();

	return result;
}


void cController::Render()
{
	RET(!m_model);
	m_model->Render();
}


void cController::Update(const float elapseT)
{
	RET(!m_model);

	if (m_isPlay)
		m_model->Move(elapseT);
	else
		m_model->Move(0);
}


void cController::SetCurrentAnimationFrame(const int curFrame)
{
	RET(!m_model);
	graphic::cBoneMgr *boneMgr = m_model->GetBoneMgr();
	RET(!boneMgr);
	boneMgr->SetCurrentAnimationFrame(curFrame);
}
