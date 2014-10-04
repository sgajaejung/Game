// 조명을 관리하는 클래스
#pragma once


namespace graphic
{


	class cLightManager : public common::cSingleton<cLightManager>
	{
	public:
		cLightManager();
		virtual ~cLightManager();

		int AddLight( const cLight &light );
		cLight& GetMainLight();
		cLight* GetLight(const int index);

		bool Bind(const int index=0);


	private:
		vector<cLight> m_lights;
	};


}
