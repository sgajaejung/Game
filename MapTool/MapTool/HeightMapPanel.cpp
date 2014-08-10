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
}


BEGIN_MESSAGE_MAP(CHeightMapPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHeightMapPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHeightMapPanel::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnDblclkListHeightmap)
	ON_LBN_SELCHANGE(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnSelchangeListHeightmap)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST_HEIGHTMAP_TEX, &CHeightMapPanel::OnSelchangeListHeightmapTex)
	ON_LBN_DBLCLK(IDC_LIST_HEIGHTMAP_TEX, &CHeightMapPanel::OnDblclkListHeightmapTex)
END_MESSAGE_MAP()


// CHeightMapPanel 메시지 처리기입니다.
BOOL CHeightMapPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateHeightMapList();
	UpdateTextureList();

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
void CHeightMapPanel::Update()
{

}
