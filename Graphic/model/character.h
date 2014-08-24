#pragma once

#include "../action/action.h"


namespace graphic
{

	DECLARE_TYPE_NAME_SCOPE(graphic, cCharacter)
	class cCharacter : public cModel
								, public memmonitor::Monitor<cCharacter, TYPE_NAME(cCharacter)>
	{
	public:
		cCharacter(const int id);
		virtual ~cCharacter();

		bool Create(const string &modelName);
		void LoadWeapon(const string &fileName);
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &tm) override;

		void SetActionData(const vector<sActionData> &actions);
		void Action(const CHARACTER_ACTION::TYPE type, const int param1=0, const int param2=0);

		// debug ¿ë ÇÔ¼ö.
		void SetRenderWeaponBoundingBox(const bool isRenderBoundingBox);


	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode1; // reference
		cBoneNode *m_weaponNode2; // reference
		cBoneNode *m_weaponBoneNode1; // reference
		cBoneNode *m_weaponBoneNode2; // reference

		vector<sActionData> m_actions;
	};

}
