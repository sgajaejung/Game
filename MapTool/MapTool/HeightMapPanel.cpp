// HeightMapPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "HeightMapPanel.h"
#include "afxdialogex.h"


// CHeightMapPanel 대화 상자입니다.
CHeightMapPanel::CHeightMapPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHeightMapPanel::IDD, pParent)
,	m_heightMap(NULL)
,	m_texture(NULL)
, m_heightFactor(0)
, m_uvFactor(0)
{

}

CHeightMapPanel::~CHeightMapPanel()
{
}

void CHeightMapPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HEIGHTMAP, m_HeightMapList);
	DDX_Control(pDX, IDC_LIST_HEIGHTMAP_TEX, m_TextureList);
	DDX_Control(pDX, IDC_SLIDER_HEIGHT_FACTOR, m_heightSlider);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_FACTOR, m_heightFactor);
	DDX_Control(pDX, IDC_SLIDER_UV_FACTOR, m_uvSlider);
	DDX_Text(pDX, IDC_EDIT_UV_FACTOR, m_uvFactor);
}


BEGIN_MESSAGE_MAP(CHeightMapPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHeightMapPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHeightMapPanel::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnDblclkListHeightmap)
	ON_LBN_SELCHANGE(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnSelchangeListHeightmap)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST_HEIGHTMAP_TEX, &CHeightMapPanel::OnSelchangeListHeightmapTex)
	ON_LBN_DBLCLK(IDC_LIST_HEIGHTMAP_TEX, &CHeightMapPanel::OnDblclkListHeightmapTex)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HEIGHT_FACTOR, &CHeightMapPanel::OnNMCustomdrawSliderHeightFactor)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT_FACTOR, &CHeightMapPanel::OnEnChangeEditHeightFactor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_UV_FACTOR, &CHeightMapPanel::OnNMCustomdrawSliderUvFactor)
	ON_EN_CHANGE(IDC_EDIT_UV_FACTOR, &CHeightMapPanel::OnEnChangeEditUvFactor)
END_MESSAGE_MAP()


// CHeightMapPanel 메시지 처리기입니다.
BOOL CHeightMapPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_heightSlider.SetRange(0, 1000);
	m_uvSlider.SetRange(0, 1000);

	UpdateHeightMapList();
	UpdateTextureList();
	UpdateTerrainInfo();

	return TRUE;
}


void CHeightMapPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Graphics *graph = Graphics::FromHDC(dc.GetSafeHdc());

	if (m_heightMap)
	{
		// 텍스쳐 출력.
		if (CWnd *pictureCtlr = GetDlgItem(IDC_STATIC_TEXTURE))
		{
			CRect cr;
			pictureCtlr->GetWindowRect(cr);
			ScreenToClient(cr);

			const Rect dest(cr.left, cr.top, cr.Width(), cr.Height());
			graph->DrawImage(m_heightMap, dest );
		}
	}

	if (m_texture)
	{
		// 텍스쳐 출력.
		if (CWnd *pictureCtlr = GetDlgItem(IDC_STATIC_TEXTURE2))
		{
			CRect cr;
			pictureCtlr->GetWindowRect(cr);
			ScreenToClient(cr);

			const Rect dest(cr.left, cr.top, cr.Width(), cr.Height());
			graph->DrawImage(m_texture, dest );
		}
	}
}


void CHeightMapPanel::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CHeightMapPanel::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


void CHeightMapPanel::UpdateTerrainInfo()
{
	m_heightFactor = cMapController::Get()->GetTerrain().GetHeightFactor();
	m_heightSlider.SetPos( (int)(m_heightFactor * 100.f) );

	m_uvFactor = cMapController::Get()->GetTerrain().GetTextureUVFactor();
	m_uvSlider.SetPos( (int)(m_uvFactor * 100.f) );

	UpdateData(FALSE);
}


void CHeightMapPanel::UpdateHeightMapList()
{
	// 리스트 박스 초기화.
	while (0 < m_HeightMapList.GetCount())
		m_HeightMapList.DeleteString(0);

	// 파일 찾기.
	list<string> extList;
	extList.push_back("jpg");
	extList.push_back("png");
	extList.push_back("bmp");
	list<string> heightMapFiles;
	common::CollectFiles(extList, "../../media/terrain/", heightMapFiles);

	BOOST_FOREACH(auto &fileName, heightMapFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_HeightMapList.InsertString(m_HeightMapList.GetCount(), wstr.c_str());
	}
}


// 지형 텍스쳐 리스트 출력.
void CHeightMapPanel::UpdateTextureList()
{
	// 리스트 박스 초기화.
	while (0 < m_TextureList.GetCount())
		m_TextureList.DeleteString(0);

	// 파일 찾기.
	list<string> extList;
	extList.push_back("jpg");
	extList.push_back("png");
	extList.push_back("bmp");
	list<string> textureFiles;
	common::CollectFiles(extList, "../../media/terrain/", textureFiles);

	BOOST_FOREACH(auto &fileName, textureFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_TextureList.InsertString(m_TextureList.GetCount(), wstr.c_str());
	}
}


// 지형 높이맵 설정.
void CHeightMapPanel::OnDblclkListHeightmap()
{
	RET(m_HeightMapList.GetCurSel() < 0);

	CString fileName;
	m_HeightMapList.GetText(m_HeightMapList.GetCurSel(), fileName);
	SAFE_DELETE(m_heightMap);
	m_heightMap = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);

	string asciiFileName = wstr2str((wstring)fileName);
	cMapController::Get()->LoadHeightMap(asciiFileName);
}


// 지형 높이맵 이미지 업데이트
void CHeightMapPanel::OnSelchangeListHeightmap()
{
	RET(m_HeightMapList.GetCurSel() < 0);

	CString fileName;
	m_HeightMapList.GetText(m_HeightMapList.GetCurSel(), fileName);
	SAFE_DELETE(m_heightMap);
	m_heightMap = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);
}


// 지형 텍스쳐 이미지 업데이트
void CHeightMapPanel::OnSelchangeListHeightmapTex()
{
	RET(m_TextureList.GetCurSel() < 0);

	CString fileName;
	m_TextureList.GetText(m_TextureList.GetCurSel(), fileName);
	SAFE_DELETE(m_texture);
	m_texture = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);	
}


// 지형 텍스쳐 설정.
void CHeightMapPanel::OnDblclkListHeightmapTex()
{
	RET(m_TextureList.GetCurSel() < 0);

	CString fileName;
	m_TextureList.GetText(m_TextureList.GetCurSel(), fileName);
	SAFE_DELETE(m_texture);
	m_texture = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);

	string asciiFileName = wstr2str((wstring)fileName);
	cMapController::Get()->LoadHeightMapTexture(asciiFileName);
}


// MapController 가 업데이트 될 때 호출된다.
void CHeightMapPanel::Update(int type)
{

	switch (type)
	{
	case NOTIFY_TYPE::NOTIFY_CHANGE_TERRAIN:
		UpdateTerrainInfo();
		break;
	}

}


void CHeightMapPanel::OnNMCustomdrawSliderHeightFactor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	m_heightFactor = m_heightSlider.GetPos() / 100.f;
	cMapController::Get()->UpdateHeightFactor(m_heightFactor);
	UpdateData(FALSE);
	*pResult = 0;
}


void CHeightMapPanel::OnEnChangeEditHeightFactor()
{
	UpdateData();
	m_heightSlider.SetPos( m_heightFactor * 100.f );
	cMapController::Get()->UpdateHeightFactor(m_heightFactor);
}


void CHeightMapPanel::OnNMCustomdrawSliderUvFactor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	m_uvFactor = m_uvSlider.GetPos() / 100.f;
	cMapController::Get()->GetTerrain().SetTextureUVFactor(m_uvFactor);
	UpdateData(FALSE);
	*pResult = 0;
}


void CHeightMapPanel::OnEnChangeEditUvFactor()
{
	UpdateData();
	m_uvSlider.SetPos( m_uvFactor * 100.f );
	cMapController::Get()->GetTerrain().SetTextureUVFactor(m_uvFactor);
}
