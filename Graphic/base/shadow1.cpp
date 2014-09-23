
#include "stdafx.h"
#include "shadow1.h"

using namespace graphic;


cShadow1::cShadow1()
{
}

cShadow1::~cShadow1()
{
}


// 그림자 클래스 생성.
bool cShadow1::Create(const int textureWidth, const int textureHeight)
{
	return m_surface.CreateRenderTarget(textureWidth, textureHeight);
}


// 그림자 텍스쳐를 업데이트 한다.
void cShadow1::UpdateShadow( cNode &node )
{
	RET(!m_surface.IsLoaded());

	const Vector3 pos = node.GetTM().GetPosition();

	// 전역 광원으로 부터 그림자 생성에 필요한 정보를 얻어온다.
	Vector3 lightPos;
	Matrix44 view, proj, tt;
	cLightManager::Get()->GetMainLight().GetShadowMatrix(
		pos, lightPos, view, proj, tt );

	m_surface.Begin();

	GetDevice()->Clear(0L, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, 0x00000000, 1.0f, 0L);

	node.RenderShadow(view*proj, lightPos, Vector3(0,-1,0), Matrix44::Identity);

	m_surface.End();
}


// 그림자 맵 출력. (디버깅용)
void cShadow1::RenderShadowMap()
{
	m_surface.Render();
}
