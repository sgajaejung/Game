// 전역 카메라 클래스.
//

#pragma once


namespace graphic
{

	class cMainCamera : public cCamera
									, public common::cSingleton<cMainCamera>
	{
	public:
		cMainCamera();
		virtual ~cMainCamera();

	};

}
