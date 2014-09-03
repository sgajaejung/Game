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
			Vector3 pos;
			Vector3 velocity;
		};


	public:
		cParticles();
		virtual ~cParticles();

		bool Create( const string &textureFileName, const int maxParticle );
		void Emit( const int count, const float interval, const float life, const float size );
		void Move( const float elapseTime);
		void Render();


	private:
		float m_size;
		DWORD m_dwFlush;
		DWORD m_dwDiscard;

		vector<sParticle> m_particles;
		cVertexBuffer m_vtxBuffer;
		cTexture *m_texture; // reference
	};


}
