
#include "stdafx.h"
#include "teracharacter.h"

using namespace graphic;


cTeraCharacter::cTeraCharacter() : 
	cCharacter(common::GenerateId())
,	m_bodyNeckNode(NULL)
,	m_bodyFaceNode(NULL)
,	m_faceFaceNode(NULL)
,	m_faceNeckNode(NULL)
,	m_hairHairNode(NULL)
{
	ZeroMemory(m_models, sizeof(m_models));

}

cTeraCharacter::~cTeraCharacter()
{
	for (int i=0; i < TERA_MODEL::MAX_TYPE; ++i)
		SAFE_DELETE(m_models[ i]);
}


bool cTeraCharacter::Create( 
	const string &faceModelFileName,
	const string &hairModelFileName,
	const string &bodyModelFileName,
	const string &handModelFileName,
	const string &legModelFileName,
	const string &tailModelFileName )
{
	if (!faceModelFileName.empty())
	{
		SetFaceModel(faceModelFileName);
	}

	if (!hairModelFileName.empty())
	{
		SetHairModel(hairModelFileName);
	}

	if (!bodyModelFileName.empty())
	{
		SetBodyModel(bodyModelFileName);
	}

	if (!handModelFileName.empty())
	{
		SetHandModel(handModelFileName);
	}

	if (!legModelFileName.empty())
	{
		SetLegModel(legModelFileName);
	}

	return true;
}


// 에니메이션
bool cTeraCharacter::SetAnimation( const string &aniFileName )
{
	if (m_models[ TERA_MODEL::BODY])
		m_models[ TERA_MODEL::BODY]->SetAnimation(aniFileName);
	if (m_models[ TERA_MODEL::HAND])
		m_models[ TERA_MODEL::HAND]->SetAnimation(aniFileName);
	if (m_models[ TERA_MODEL::LEG])
		m_models[ TERA_MODEL::LEG]->SetAnimation(aniFileName);

	if (m_models[ TERA_MODEL::FACE])
	{
		//m_models[ TERA_MODEL::FACE]->SetAnimation("popori_face_wait.ani");
		m_models[ TERA_MODEL::FACE]->GetBoneMgr()->SetAnimationOption( 0x01 );
	}

	return true;
}


void cTeraCharacter::Render(const Matrix44 &tm)
{
	if (m_models[ TERA_MODEL::BODY])
		m_models[ TERA_MODEL::BODY]->Render(m_TM*tm);
	if (m_models[ TERA_MODEL::HAND])
		m_models[ TERA_MODEL::HAND]->Render(m_TM*tm);
	if (m_models[ TERA_MODEL::LEG])
		m_models[ TERA_MODEL::LEG]->Render(m_TM*tm);

	if (m_models[ TERA_MODEL::FACE])
	{
		m_models[ TERA_MODEL::FACE]->Render(m_TM*tm);
	}

	if (m_models[ TERA_MODEL::HAIR])
	{
		m_models[ TERA_MODEL::HAIR]->Render(m_TM*tm);
	}

}


bool cTeraCharacter::Move(const float elapseTime)
{
	if (m_models[ TERA_MODEL::BODY])
		m_models[ TERA_MODEL::BODY]->Move(elapseTime);

	if (m_faceNeckNode)
		m_faceNeckNode->SetLocalTM( m_bodyNeckNode->GetAccTM() );
	if (m_faceFaceNode)
		m_faceFaceNode->SetLocalTM( m_bodyFaceNode->GetCalculateAniTM() );

	if (m_models[ TERA_MODEL::FACE])
		m_models[ TERA_MODEL::FACE]->Move(elapseTime);

	if (m_hairHairNode)
		m_hairHairNode->SetLocalTM( m_faceFaceNode->GetAccTM() );

	if (m_models[ TERA_MODEL::HAIR])
		m_models[ TERA_MODEL::HAIR]->Move(elapseTime);

	return true;
}


void cTeraCharacter::SetBodyModel( const string &fileName )
{
	if (!m_models[TERA_MODEL::BODY])
		m_models[TERA_MODEL::BODY] = new cModel(common::GenerateId());

	m_models[ TERA_MODEL::BODY]->Create(fileName);
	//m_models[ TERA_MODEL::BODY]->SetRenderBone(true);

	m_bodyNeckNode = m_models[ TERA_MODEL::BODY]->GetBoneMgr()->FindBone( "Bip01-Neck" );
	m_bodyFaceNode = m_models[ TERA_MODEL::BODY]->GetBoneMgr()->FindBone( "Bip01-Head" );
}


void cTeraCharacter::SetHandModel( const string &fileName )
{
	if (!m_models[TERA_MODEL::HAND])
		m_models[TERA_MODEL::HAND] = new cModel(common::GenerateId());

	m_models[ TERA_MODEL::HAND]->Create(fileName);
	m_models[ TERA_MODEL::HAND]->SharePalette( &m_models[ TERA_MODEL::BODY]->GetBoneMgr()->GetPalette() );
}


void cTeraCharacter::SetLegModel( const string &fileName )
{
	if (!m_models[TERA_MODEL::LEG])
		m_models[TERA_MODEL::LEG] = new cModel(common::GenerateId());

	m_models[ TERA_MODEL::LEG]->Create(fileName);
	m_models[ TERA_MODEL::LEG]->SharePalette( &m_models[ TERA_MODEL::BODY]->GetBoneMgr()->GetPalette() );
}


void cTeraCharacter::SetFaceModel( const string &fileName )
{
	if (!m_models[TERA_MODEL::FACE])
		m_models[TERA_MODEL::FACE] = new cModel(common::GenerateId());

	m_models[ TERA_MODEL::FACE]->Create(fileName);

	m_faceNeckNode = m_models[ TERA_MODEL::FACE]->GetBoneMgr()->FindBone( "Dummy_Neck" );
	m_faceFaceNode = m_models[ TERA_MODEL::FACE]->GetBoneMgr()->FindBone( "Dummy_Face" );
}


void cTeraCharacter::SetHairModel( const string &fileName )
{
	if (!m_models[TERA_MODEL::HAIR])
		m_models[TERA_MODEL::HAIR] = new cModel(common::GenerateId());

	m_models[ TERA_MODEL::HAIR]->Create(fileName);
	m_hairHairNode = m_models[ TERA_MODEL::HAIR]->GetBoneMgr()->FindBone( "Dummy_Hair" );
}


void cTeraCharacter::SetTailModel( const string &fileName )
{

}
