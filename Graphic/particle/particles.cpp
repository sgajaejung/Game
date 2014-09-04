
#include "stdafx.h"
#include "particles.h"

using namespace graphic;

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


cParticles::cParticles()
	: m_particleCount(0)
,	m_pointSize(1)
,	m_texture(NULL)
,	m_gravity(0, 0, 0)
,	m_wind(0,0,0)
,	m_bAirResistence(false)
{

}

cParticles::~cParticles()
{

}


// 파티클 초기화.
// 원본 코드는 D3DPOOL_DEFAULT 플래그를 이용해서 프래임을 높이려고 애쓰는
// 흔적이 많은데, 테스트 결과 거의 효과가 없어서, 그 부분을 제외 했다.
// 괜히 코드만 더 복잡해진다.
bool cParticles::Create( const string &textureFileName, const int maxParticle )
{
	m_vtxBuffer.Create(maxParticle, sizeof(sVertexDiffuse), sVertexDiffuse::FVF);
	m_particles.resize(maxParticle);
	m_texture = cResourceManager::Get()->LoadTexture(textureFileName);

	return true;
}


void cParticles::Emit( const int count, const float interval, const float life, const float size )
{
	m_particles[ 0].velocity = Vector3(0,10,0);
	m_particles[ 1].velocity = Vector3(1,10,0);
	m_particles[ 2].velocity = Vector3(-1,10,0);
}


void cParticles::Move( const float elapseTime )
{

	if (sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuffer.Lock())
	{
		for (u_int i=0; i < m_particles.size(); ++i)
		{
			sParticle &particle = m_particles[ i];

			particle.velocity += m_gravity * elapseTime;

            if (m_bAirResistence)
                particle.velocity += (m_wind - particle.velocity) * elapseTime;

			particle.pos += particle.velocity * elapseTime;

			vertices->p = particle.pos;
			vertices->c = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			++vertices;
		}
	}

}


void cParticles::Render()
{
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);

	GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );       // Turn on point sprites
	GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );       // Allow sprites to be scaled with distance
	GetDevice()->SetRenderState( D3DRS_POINTSIZE,     FtoDW(m_pointSize) ); // Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex.
	GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(1.0f) );    // Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.0f) );    // Default 1.0
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.0f) );    // Default 0.0
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) );    // Default 0.0

	if (m_texture)
		m_texture->Bind(0);
	m_vtxBuffer.RenderPointList(3);//m_particleCount);

	m_texture->Unbind(0);
	GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE);
}

