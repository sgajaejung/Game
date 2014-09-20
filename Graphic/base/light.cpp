
#include "stdafx.h"
#include "light.h"

using namespace graphic;

cLight::cLight()
{

}


cLight::~cLight()
{

}


void cLight::Init(TYPE type, 
	const Vector4 &ambient, // Vector4(1, 1,13, 1),
	const Vector4 &diffuse, // Vector4(0.2, 0.2, 0.2, 1)
	const Vector4 &specular, // Vector4(1,1,1,1)
	const Vector3 &direction) // Vector3(0,-1,0)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = (D3DLIGHTTYPE)type;
	m_light.Ambient = *(D3DCOLORVALUE*)&ambient;
	m_light.Diffuse = *(D3DCOLORVALUE*)&diffuse;
	m_light.Specular = *(D3DCOLORVALUE*)&specular;
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}


void cLight::Bind(int lightIndex)
{
	GetDevice()->SetLight(lightIndex, &m_light); // 광원 설정.
}


void cLight::SetDirection( const Vector3 &direction )
{
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}


void cLight::SetPosition( const Vector3 &pos )
{
	m_light.Position = *(D3DXVECTOR3*)&pos;
}


// 그림자를 출력하기 위한 정보를 리턴한다.
// modelPos : 그림자를 출력할 모델의 위치 (월드상의)
// lightPos : 광원의 위치가 저장되어 리턴.
// view : 광원에서 모델을 바라보는 뷰 행렬
// proj : 광원에서 모델을 바라보는 투영 행렬
// tt : 투영 좌표에서 텍스쳐 좌표로 변환하는 행렬.
void cLight::GetShadowMatrix( const Vector3 &modelPos, 
	OUT Vector3 &lightPos, OUT Matrix44 &view, OUT Matrix44 &proj, 
	OUT Matrix44 &tt )
{
	lightPos = *(Vector3*)&m_light.Position;

	view.SetView2( lightPos, modelPos, Vector3(0,1,0));

	proj.SetProjection( D3DX_PI/8.f, 1, 0.1f, 10000);

	D3DXMATRIX mTT= D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f
		, 0.0f,-0.5f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.5f, 0.5f, 0.0f, 1.0f);
	tt = *(Matrix44*)&mTT;
}

