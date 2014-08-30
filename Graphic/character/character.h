#pragma once

#include "../action/action.h"


namespace graphic
{

	class cCharacter : public cModel
	{
	public:
		cCharacter(const int id);
		virtual ~cCharacter();

		void SetActionData(const vector<sActionData> &actions);
		void Action(const CHARACTER_ACTION::TYPE type, const int param1=0, const int param2=0);


	private:
		// action
		vector<sActionData> m_actions;
		CHARACTER_ACTION::TYPE m_action;
	};

}
