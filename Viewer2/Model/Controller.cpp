
#include "stdafx.h"
#include "Controller.h"



cController::cController() : 
	m_model(NULL)
{
	m_model = new graphic::cModel();
}

cController::~cController()
{
	SAFE_DELETE(m_model);
}


bool cController::LoadFile( const string &fileName )
{
	const graphic::RESOURCE_TYPE::TYPE type = graphic::cResourceManager::Get()->GetFileKind(fileName);
	switch (type)
	{
	case graphic::RESOURCE_TYPE::MESH:
		return m_model->Create(fileName);

	case graphic::RESOURCE_TYPE::ANIMATION:
		m_model->SetAnimation(fileName);
		return true;
	}

	return true;
}
