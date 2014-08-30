#pragma once


namespace graphic
{

	namespace TERA_MODEL
	{
		enum TYPE
		{
			FACE, 
			HAIR,
			BODY,
			HAND,
			LEG,
			TAIL,
			MAX_TYPE
		};
	}


	class cTeraCharacter
	{
	public:
		cTeraCharacter();
		virtual ~cTeraCharacter();

		bool Create( 
			const string &faceModelFileName,
			const string &hairModelFileName,
			const string &bodyModelFileName,
			const string &handModelFileName,
			const string &legModelFileName,
			const string &tailModelFileName );

		bool SetAnimation( const string &aniFileName );

		void Render();
		void Move(const float elapseTime);



	private:
		cModel *m_models[ TERA_MODEL::MAX_TYPE];

		cBoneNode *m_bodyNeckNode; // reference
		cBoneNode *m_bodyFaceNode; // reference
		cBoneNode *m_faceFaceNode; // reference

		cBoneNode *m_faceNeckNode; // reference
		cBoneNode *m_hairHairNode; // reference

	};

}
