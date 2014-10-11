// 이동 행동.

#pragma once

#include "action.h"

namespace ai
{
	class cAction;
	class IArtificialIntelligence;

	class cMove : public cAction
	{
	public:
		cMove();

		void Init(IArtificialIntelligence *obj, const Vector3 &dest);
		virtual bool ActionUpdate(const float elapseTime) override;
		virtual void Start(cAction *prevAction) override;


	private:
		Vector3 m_dest;
		Vector3 m_dir;
	};

}
