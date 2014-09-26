
#include "stdafx.h"
#include "surface2.h"

using namespace graphic;


cSurface2::cSurface2()
	: m_texture(NULL)
,	m_rts(NULL)
,	m_surface(NULL)
,	m_width(0)
,	m_height(0)
,	m_mipLevels(0)
,	m_autoGenMips(0)
{
}

cSurface2::~cSurface2()
{
	Clear();
}


bool cSurface2::Create(int width, int height, int mipLevels,
	D3DFORMAT texFormat, bool useDepthBuffer,
	D3DFORMAT depthFormat, D3DVIEWPORT9& viewport, bool autoGenMips)
{
	m_width = width;
	m_height = height;
	m_mipLevels = mipLevels;
	m_texFormat = texFormat;
	m_useDepthBuffer = useDepthBuffer;
	m_depthFormat = depthFormat;
	m_viewPort = viewport;
	m_autoGenMips = autoGenMips;

	UINT usage = D3DUSAGE_RENDERTARGET;
	if(m_autoGenMips)
		usage |= D3DUSAGE_AUTOGENMIPMAP;

	if (FAILED(D3DXCreateTexture(GetDevice(), width, height, mipLevels, 
		usage, texFormat, D3DPOOL_DEFAULT, &m_texture)))
	{
		return false;
	}

	if (FAILED(D3DXCreateRenderToSurface(GetDevice(), width, height, texFormat, 
		useDepthBuffer, depthFormat, &m_rts)))
	{
		return false;
	}

	if (FAILED(m_texture->GetSurfaceLevel(0, &m_surface)))
	{
		return false;
	}

	return true;
}


void cSurface2::Begin()
{
	m_rts->BeginScene(m_surface, &m_viewPort);
}


void cSurface2::End()
{
	m_rts->EndScene(D3DX_FILTER_NONE);
}


void cSurface2::Render()
{

}


void cSurface2::Clear()
{
	SAFE_RELEASE(m_texture);
	SAFE_RELEASE(m_rts);
	SAFE_RELEASE(m_surface);
}
