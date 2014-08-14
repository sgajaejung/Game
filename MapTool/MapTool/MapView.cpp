// MapView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapView.h"


using namespace graphic;


// CMapView
CMapView::CMapView() :
	m_dxInit(false)
,	m_RButtonDown(false)
,	m_LButtonDown(false)
,	m_MButtonDown(false)
,	m_modelShader(NULL)
{

}

CMapView::~CMapView()
{
}

BEGIN_MESSAGE_MAP(CMapView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMapView 그리기입니다.

void CMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMapView 진단입니다.

#ifdef _DEBUG
void CMapView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapView 메시지 처리기입니다.

bool CMapView::Init()
{
	graphic::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	graphic::GetDevice()->LightEnable (
		0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
		true); // true = 활성화 ， false = 비활성화


	m_grid.Create(64,64,50.f);
	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));
	m_cube.SetColor( 0xFF0000FF );
	m_dxInit = true;

	m_terrainShader.Create( "../../media/shader/hlsl_terrain.fx", "TShader" );
	m_terrainShader2.Create( "../../media/shader/hlsl_terrain_splatting.fx", "TShader" );
	m_modelShader = graphic::cResourceManager::Get()->LoadShader(  "hlsl_skinning_no_light.fx" );

	return true;
}


void CMapView::Render()
{
	if (!m_dxInit)
		return;

	//화면 청소
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		graphic::GetDevice()->BeginScene();
		graphic::GetRenderer()->RenderFPS();
		//graphic::GetRenderer()->RenderGrid();

		const Matrix44 matIdentity;

		cCamera &camera = cMapController::Get()->GetCamera();
		m_terrainShader.SetMatrix( "mVP", camera.GetViewProjectionMatrix());
		m_terrainShader.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_terrainShader.SetVector( "vEyePos", camera.GetEyePos());
		m_terrainShader.SetMatrix( "mWIT", matIdentity);
		m_terrainShader.SetMatrix( "mWorld", matIdentity);
		//m_terrainShader.SetTexture("ShadowMap", m_pShadowTex);

		m_modelShader->SetMatrix( "mVP", camera.GetViewProjectionMatrix());
		m_modelShader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_modelShader->SetVector( "vEyePos", camera.GetEyePos());

		m_terrainShader2.SetMatrix( "mVP", camera.GetViewProjectionMatrix());
		m_terrainShader2.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_terrainShader2.SetVector( "vEyePos", camera.GetEyePos());
		m_terrainShader2.SetMatrix( "mWIT", matIdentity);
		m_terrainShader2.SetMatrix( "mWorld", matIdentity);
		//m_terrainShader.2SetTexture("ShadowMap", m_pShadowTex);

		//m_terrainShader.SetRenderPass(1);
		//cMapController::Get()->GetTerrain().Render();
		//cMapController::Get()->GetTerrain().RenderShader(m_terrainShader);

		m_terrainShader2.SetRenderPass(1);
		cMapController::Get()->GetTerrain().RenderShader(m_terrainShader2);


		switch (cMapController::Get()->GetEditMode())
		{
		case EDIT_MODE::MODE_BRUSH:
			cMapController::Get()->GetTerrainCursor().RenderBrush();
			break;
		case EDIT_MODE::MODE_MODEL:
			cMapController::Get()->GetTerrainCursor().RenderModel();
			break;
		}


		graphic::GetRenderer()->RenderAxis();

		//랜더링 끝
		graphic::GetDevice()->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}

}


void CMapView::Update(float elapseT)
{
	graphic::GetRenderer()->Update(elapseT);
}


void CMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_LButtonDown = true;
	m_curPos = point;
	CView::OnLButtonDown(nFlags, point);
}


void CMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	// 지형위에 모델을 위치 시킨다.
	if (m_LButtonDown && 
		(cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_MODEL))
	{
		// 모델이 선택되어 있는 상태라면, 모델을 지형위에 위치 시킨다.
		if (cMapController::Get()->GetTerrainCursor().IsSelectModel())
		{
			if (const graphic::cModel *model = cMapController::Get()->GetTerrainCursor().GetSelectModel())
			{
				cMapController::Get()->GetTerrain().AddRigidModel(*model);
				cMapController::Get()->UpdatePlaceModel();
			}
		}
		else
		{
			// 모델이 선택되어 있지 않다면, 지형위의 모델을 피킹해서 선택한다.
			cCamera &camera = cMapController::Get()->GetCamera();
			m_ray.Create(point.x, point.y, VIEW_WIDTH, VIEW_HEIGHT, 
				camera.GetProjectionMatrix(), camera.GetViewMatrix() );

			// 모델 피킹.
			if (graphic::cModel *model = 
				cMapController::Get()->GetTerrain().PickModel(m_ray.orig, m_ray.dir))
			{
				cMapController::Get()->GetTerrain().RemoveRigidModel(model, false);
				cMapController::Get()->GetTerrainCursor().SelectModel(model);
				cMapController::Get()->UpdatePlaceModel();
			}

		}
	}

	m_LButtonDown = false;
	CView::OnLButtonUp(nFlags, point);
}


void CMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CView::OnRButtonDown(nFlags, point);
}


void CMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;	
	CView::OnRButtonUp(nFlags, point);
}


void CMapView::OnMouseMove(UINT nFlags, CPoint point)
{
	cCamera &camera = cMapController::Get()->GetCamera();

	if (m_LButtonDown)
	{
		m_curPos = point;

		if (cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_BRUSH)
		{
			cMapController::Get()->Brush(point);
		}

	}
	else if (m_RButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;
		camera.Pitch2(pos.y * 0.005f); 
		camera.Yaw2(pos.x * 0.005f); 
	}
	else if (m_MButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;

		Vector3 dir = camera.GetDirection();
		dir.y = 0;
		dir.Normalize();

		const float len = camera.GetDistance();
		camera.MoveRight( -pos.x * len * 0.001f );
		camera.MoveAxis( dir, pos.y * len * 0.001f );
	}
	else
	{
		m_curPos = point;

		if ((cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_BRUSH) ||
			(cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_MODEL))
		{
			m_ray.Create(m_curPos.x, m_curPos.y, VIEW_WIDTH, VIEW_HEIGHT, 
				camera.GetProjectionMatrix(), camera.GetViewMatrix() );

			Vector3 pickPos;
			cMapController::Get()->GetTerrain().Pick(m_ray.orig, m_ray.dir, pickPos);
			cMapController::Get()->GetTerrainCursor().UpdateCursor(cMapController::Get()->GetTerrain(), pickPos );
		}

	}

	CView::OnMouseMove(nFlags, point);
}


void CMapView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	CView::OnMButtonDown(nFlags, point);
}


void CMapView::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_MButtonDown = false;
	ReleaseCapture();
	CView::OnMButtonUp(nFlags, point);
}


BOOL CMapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	cCamera &camera = cMapController::Get()->GetCamera();
	
	const float len = camera.GetDistance();
	float zoomLen = (len > 100)? 50 : (len/4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen/10.f;

	camera.Zoom( (zDelta<0)? -zoomLen : zoomLen );	

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_TAB:
		{
			static bool flag = false;
			graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, flag? D3DCULL_CCW : D3DCULL_NONE);
			graphic::GetDevice()->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
			flag = !flag;
		}
		break;

	case VK_ESCAPE:
		{
			cMapController::Get()->GetTerrainCursor().CancelSelectModel();
		}
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
