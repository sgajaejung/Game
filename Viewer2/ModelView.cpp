// ModelView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "ModelView.h"


using namespace graphic;

// CModelView

CModelView::CModelView()
{
	m_LButtonDown = false;
	m_RButtonDown = false;
	m_MButtonDown = false;	
	m_showSkybox = false;
}

CModelView::~CModelView()
{
}

BEGIN_MESSAGE_MAP(CModelView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CModelView 진단입니다.

#ifdef _DEBUG
void CModelView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CModelView 메시지 처리기입니다.

void CModelView::Init()
{
	const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
	const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
	GetMainCamera()->SetCamera(Vector3(100,300,-500), Vector3(0,0,0), Vector3(0,1,0));
	GetMainCamera()->SetProjection( D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f) ;


	GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	GetDevice()->LightEnable (
		0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
		true); // true = 활성화 ， false = 비활성화

	// 주 광원 초기화.
	cLightManager::Get()->GetMainLight().Bind(0);


	//m_shader.Create( "../media/shader/hlsl_skinning_using_color.fx", "TShader" );
	//m_shader.Create( "../media/shader/hlsl_skinning_using_texcoord.fx", "TShader" );
	m_shader.Create( "../media/shader/hlsl_rigid_phong.fx", "TShader" );
	m_skybox.Create( "../media/skybox" );
	
	m_grid.Create(64, 64, 50, 1);
	//m_grid.GetTexture().Create( "../media/texture/transparent.png" );
	m_grid.GetTexture().Create( "../media/texture/emptyTexture2.png" );

	cController::Get()->AddObserver(this);

}


void CModelView::Update(const float elapseT)
{
	GetRenderer()->Update(elapseT);
	cController::Get()->Update(elapseT);
}


void CModelView::Render()
{
	//화면 청소
	if (SUCCEEDED(GetDevice()->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		GetDevice()->BeginScene();

		cLightManager::Get()->GetMainLight().Bind(0);


		GetDevice()->SetTransform(D3DTS_WORLD, ToDxM(Matrix44::Identity) );

		if (m_showSkybox)
			m_skybox.Render();
		//GetRenderer()->RenderFPS();
		//GetRenderer()->RenderGrid();
		//GetRenderer()->RenderAxis();

		if (cCharacter *character = cController::Get()->GetCharacter())
		{
			character->SetRenderShadow(true);
			character->SetTM(m_rotateTm);

			// 그림자 텍스쳐, 매트릭스 설정.
			Vector3 lightPos;
			Matrix44 view, proj, tt;
			cLightManager::Get()->GetMainLight().GetShadowMatrix(
				Vector3(0,0,0), lightPos, view, proj, tt );
			m_shader.SetRenderPass(3);

			m_shader.SetMatrix( "mWVPT", view * proj * tt );
			m_shader.SetTexture("ShadowMap", character->GetShadow().GetTexture());
		}

		// 바닥 그림자 출력.
		// 셰이더 변수 초기화.
		m_shader.SetMatrix( "mVP", GetMainCamera()->GetViewProjectionMatrix() );
		m_shader.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shader.SetVector( "vEyePos", GetMainCamera()->GetEyePos());
		m_shader.SetVector( "vFog", Vector3(1.f, 10000.f, 0)); // near, far

		// 바닥은 일단 z 버퍼를 쓰지않고 출력한다.
		GetDevice()->SetRenderState(D3DRS_ZENABLE, 0);
		m_grid.RenderShader(m_shader);
		GetDevice()->SetRenderState(D3DRS_ZENABLE, 1);

		// 캐릭터 출력.
		cController::Get()->Render();

		// 백그라운드 그리드, 축 출력.
		GetRenderer()->RenderFPS();
		GetRenderer()->RenderGrid();
		GetRenderer()->RenderAxis();

		//랜더링 끝
		GetDevice()->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}


void CModelView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	SetFocus();
	m_LButtonDown = true;
	m_curPos = point;
	CView::OnLButtonDown(nFlags, point);
}


void CModelView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_LButtonDown = false;
	CView::OnLButtonUp(nFlags, point);
}


void CModelView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_LButtonDown)
	{
		const CPoint pos = point  - m_curPos;
		m_curPos = point;

		Quaternion q1(GetMainCamera()->GetRight(), -pos.y * 0.01f);
		Quaternion q2(GetMainCamera()->GetUpVector(), -pos.x * 0.01f);

		m_rotateTm *= (q2.GetMatrix() * q1.GetMatrix());
	}	
	else if (m_RButtonDown)
	{
		const CPoint pos = point  - m_curPos;
		m_curPos = point;

		GetMainCamera()->Yaw2( pos.x * 0.005f );
		GetMainCamera()->Pitch2( pos.y * 0.005f );
	}
	else if (m_MButtonDown)
	{
		const CPoint pos = point  - m_curPos;
		m_curPos = point;

		const float len = GetMainCamera()->GetDistance();
		GetMainCamera()->MoveRight( -pos.x * len * 0.001f );
		GetMainCamera()->MoveUp( pos.y * len * 0.001f );
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CModelView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	dbg::Print( "%d %d", nFlags, zDelta);

	const float len = GetMainCamera()->GetDistance();
	float zoomLen = (len > 100)? 50 : (len/4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen/10.f;

	GetMainCamera()->Zoom( (zDelta<0)? -zoomLen : zoomLen );

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CModelView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CView::OnRButtonDown(nFlags, point);
}


void CModelView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CView::OnRButtonUp(nFlags, point);
}


void CModelView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	m_curPos = point;
	CView::OnMButtonDown(nFlags, point);
}


void CModelView::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_MButtonDown = false;
	CView::OnMButtonUp(nFlags, point);
}


void CModelView::Update()
{
}


void CModelView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'F')
	{ // focus model
		// 모델이 화면에 잘 보이게 자동으로 조절한다.
		if (cCharacter *character = cController::Get()->GetCharacter())
		{
			if (cBoundingBox *box = character->GetCollisionBox())
			{
				const float len = box->Length();
				
				Vector3 eyePos = GetMainCamera()->GetEyePos().Normal() * len*2;
				GetMainCamera()->SetLookAt(box->Center());
				GetMainCamera()->SetEyePos(eyePos);
			}
		}
		
	}

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

