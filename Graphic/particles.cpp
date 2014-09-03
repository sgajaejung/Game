
#include "stdafx.h"
#include "particles.h"

using namespace graphic;

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


cParticles::cParticles()
{
	m_dwFlush = 512;  // Number of point sprites to load before sending them to hardware(512 = 2048 divided into 4 chunks)
	m_dwDiscard = 2048; // Max number of point sprites the vertex buffer can load until we are forced to discard and start over

}

cParticles::~cParticles()
{

}


// 파티클 초기화.
bool cParticles::Create( const string &textureFileName, const int maxParticle )
{
	m_vtxBuffer.CreateParticle(maxParticle);

	return true;
}


void cParticles::Emit( const int count, const float interval, const float life, const float size )
{


}


void cParticles::Move( const float elapseTime)
{

}


// Note: D3DLOCK_DISCARD:
//
//       The application overwrites, with a write-only operation, the entire 
//       index buffer. This enables Direct3D to return a pointer to a new 
//       memory area so that the dynamic memory access (DMA) and rendering 
//       from the old area do not stall.
//
//       D3DLOCK_NOOVERWRITE:
//
//       Indicates that no vertices that were referred to in drawing calls 
//       since the start of the frame or the last lock without this flag will 
//       be modified during the lock. This can enable optimizations when the 
//       application is appending data only to the vertex buffer. 
void cParticles::Render()
{
	GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );       // Turn on point sprites
	GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );       // Allow sprites to be scaled with distance
	GetDevice()->SetRenderState( D3DRS_POINTSIZE,     FtoDW(m_size) ); // Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex.
	GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(1.0f) );    // Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.0f) );    // Default 1.0
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.0f) );    // Default 0.0
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) );    // Default 0.0






	GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );
}

