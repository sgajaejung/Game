// 파티클을 관리하는 클래스다.
// 
// 참조 코드.
// http://www.codesampler.com/dx9src/dx9src_7.htm
#pragma once


namespace graphic
{


	class cParticles
	{
		struct sParticle
		{
			bool enable;
			Vector3 pos;
			Vector3 velocity;
			float initTime;
		};


	public:
		cParticles();
		virtual ~cParticles();

		bool Create( const string &textureFileName, const int maxParticle );
		void Emit( const int count, const float interval, const float life, const float size );
		
		virtual void Move( const float elapseTime);
		virtual void Render();


	private:
		int m_particleCount;
		float m_pointSize;
		Vector3 m_gravity;
		Vector3 m_wind;
		bool m_bAirResistence;

		vector<sParticle> m_particles;
		cVertexBuffer m_vtxBuffer;
		cTexture *m_texture; // reference
	};


}
