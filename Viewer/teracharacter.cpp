
#include "stdafx.h"
#include "teracharacter.h"

using namespace graphic;


cTeraCharacter::cTeraCharacter()
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
		if (!m_models[TERA_MODEL::FACE])
			m_models[TERA_MODEL::FACE] = new cModel(common::GenerateId());

		m_models[ TERA_MODEL::FACE]->Create(faceModelFileName);

		m_faceNeckNode = m_models[ TERA_MODEL::FACE]->GetBoneMgr()->FindBone( "Dummy_Neck" );
		m_faceFaceNode = m_models[ TERA_MODEL::FACE]->GetBoneMgr()->FindBone( "Dummy_Face" );
	}

	if (!hairModelFileName.empty())
	{
		if (!m_models[TERA_MODEL::HAIR])
			m_models[TERA_MODEL::HAIR] = new cModel(common::GenerateId());

		m_models[ TERA_MODEL::HAIR]->Create(hairModelFileName);
		m_hairHairNode = m_models[ TERA_MODEL::HAIR]->GetBoneMgr()->FindBone( "Dummy_Hair" );
	}

	if (!bodyModelFileName.empty())
	{
		if (!m_models[TERA_MODEL::BODY])
			m_models[TERA_MODEL::BODY] = new cModel(common::GenerateId());

		m_models[ TERA_MODEL::BODY]->Create(bodyModelFileName);
		//m_models[ TERA_MODEL::BODY]->SetRenderBone(true);

		m_bodyNeckNode = m_models[ TERA_MODEL::BODY]->GetBoneMgr()->FindBone( "Bip01-Neck" );
		m_bodyFaceNode = m_models[ TERA_MODEL::BODY]->GetBoneMgr()->FindBone( "Bip01-Head" );
	}

	if (!handModelFileName.empty())
	{
		if (!m_models[TERA_MODEL::HAND])
			m_models[TERA_MODEL::HAND] = new cModel(common::GenerateId());

		m_models[ TERA_MODEL::HAND]->Create(handModelFileName);
		m_models[ TERA_MODEL::HAND]->SharePalette( &m_models[ TERA_MODEL::BODY]->GetBoneMgr()->GetPalette() );
	}

	if (!legModelFileName.empty())
	{
		if (!m_models[TERA_MODEL::LEG])
			m_models[TERA_MODEL::LEG] = new cModel(common::GenerateId());

		m_models[ TERA_MODEL::LEG]->Create(legModelFileName);
		m_models[ TERA_MODEL::LEG]->SharePalette( &m_models[ TERA_MODEL::BODY]->GetBoneMgr()->GetPalette() );
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


void cTeraCharacter::Render()
{
	if (m_models[ TERA_MODEL::BODY])
		m_models[ TERA_MODEL::BODY]->Render(Matrix44::Identity);
	if (m_models[ TERA_MODEL::HAND])
		m_models[ TERA_MODEL::HAND]->Render(Matrix44::Identity);
	if (m_models[ TERA_MODEL::LEG])
		m_models[ TERA_MODEL::LEG]->Render(Matrix44::Identity);

	if (m_models[ TERA_MODEL::FACE])
	{
		m_models[ TERA_MODEL::FACE]->Render(Matrix44::Identity);
	}

	if (m_models[ TERA_MODEL::HAIR])
	{
		m_models[ TERA_MODEL::HAIR]->Render(Matrix44::Identity);
	}

}


void cTeraCharacter::Move(const float elapseTime)
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
}
