// BrushPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "BrushPanel.h"
#include "afxdialogex.h"


// CBrushPanel 대화 상자입니다.

CBrushPanel::CBrushPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBrushPanel::IDD, pParent)
,	m_texture(NULL)
, m_innerRadius(0)
, m_outerRadius(0)
{

}

CBrushPanel::~CBrushPanel()
{
}

void CBrushPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEXTURE_FILES, m_TextureFiles);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_TEXTURE, m_textureBrowser);
	DDX_Control(pDX, IDC_LIST_LAYER, m_layerList);
	DDX_Control(pDX, IDC_SLIDER_INNER_RADIUS, m_innerRSlider);
	DDX_Control(pDX, IDC_SLIDER_OUTER_RADIUS, m_outerRSlider);
	DDX_Text(pDX, IDC_EDIT_INNER_RADIUS, m_innerRadius);
	DDX_Text(pDX, IDC_EDIT_OUTER_RADIUS2, m_outerRadius);
}


BEGIN_MESSAGE_MAP(CBrushPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBrushPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBrushPanel::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_TEXTURE_FILES, &CBrushPanel::OnSelchangeListTextureFiles)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_TEXTURE, &CBrushPanel::OnChangeMfceditbrowseTexture)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_INNER_RADIUS, &CBrushPanel::OnNMCustomdrawSliderInnerRadius)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_OUTER_RADIUS, &CBrushPanel::OnNMCustomdrawSliderOuterRadius)
	ON_EN_CHANGE(IDC_EDIT_INNER_RADIUS, &CBrushPanel::OnEnChangeEditInnerRadius)
	ON_EN_CHANGE(IDC_EDIT_OUTER_RADIUS2, &CBrushPanel::OnEnChangeEditOuterRadius2)
END_MESSAGE_MAP()


// CBrushPanel 메시지 처리기입니다.
BOOL CBrushPanel::OnInitDialog()
{
	__super::OnInitDialog();

	m_layerList.InsertColumn(0, L"Layer", LVCFMT_LEFT, 40 );
	m_layerList.InsertColumn(1, L"Texture", LVCFMT_LEFT, 300 );
	m_layerList.SetExtendedStyle(m_layerList.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT);

	m_textureBrowser.EnableFolderBrowseButton();
	m_textureBrowser.SetWindowText( L"../../media/terrain/" );
	UpdateTextureFiles("../../media/terrain/");

	graphic::cTerrainCursor &cursor = cMapController::Get()->GetTerrainCursor();
	m_innerRadius = cursor.GetInnerBrushRadius();
	m_outerRadius = cursor.GetOuterBrushRadius();

	m_innerRSlider.SetRange(0, 300);
	m_outerRSlider.SetRange(0, 300);

	m_innerRSlider.SetPos( cursor.GetInnerBrushRadius() );
	m_outerRSlider.SetPos( cursor.GetOuterBrushRadius() );

	UpdateData(FALSE);

	return TRUE;
}


void CBrushPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void CBrushPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void CBrushPanel::Update(int type)
{
	switch (type)
	{
	case NOTIFY_TYPE::NOTIFY_CHANGE_TERRAIN:
		m_layerList.DeleteAllItems();
		break;

	case NOTIFY_TYPE::NOTIFY_ADD_LAYER:
		UpdateLayerList();
		break;
	}
}


void CBrushPanel::UpdateTextureFiles(const string &directoryPath)
{
	// 리스트 박스 초기화.
	while (0 < m_TextureFiles.GetCount())
		m_TextureFiles.DeleteString(0);

	// 파일 찾기.
	list<string> extList;
	extList.push_back("jpg");
	extList.push_back("png");
	extList.push_back("bmp");
	list<string> textureFiles;
	common::CollectFiles(extList, directoryPath, textureFiles);

	BOOST_FOREACH(auto &fileName, textureFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_TextureFiles.InsertString(m_TextureFiles.GetCount(), wstr.c_str());
	}
}


void CBrushPanel::OnSelchangeListTextureFiles()
{
	RET(m_TextureFiles.GetCurSel() < 0);

	CString fileName;
	m_TextureFiles.GetText(m_TextureFiles.GetCurSel(), fileName);
	SAFE_DELETE(m_texture);
	m_texture = Image::FromFile(fileName);

	const string strFileName = wstr2str((wstring)fileName);
	cMapController::Get()->GetTerrainCursor().SelectBrushTexture(strFileName);

	InvalidateRect(NULL, FALSE);
}


void CBrushPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Graphics *graph = Graphics::FromHDC(dc.GetSafeHdc());

	if (m_texture)
	{
		// 텍스쳐 출력.
		if (CWnd *pictureCtlr = GetDlgItem(IDC_STATIC_TEXTURE))
		{
			CRect cr;
			pictureCtlr->GetWindowRect(cr);
			ScreenToClient(cr);

			const Rect dest(cr.left, cr.top, cr.Width(), cr.Height());
			graph->DrawImage(m_texture, dest );
		}
	}
}


void CBrushPanel::OnChangeMfceditbrowseTexture()
{
	CString wfilePath;
	m_textureBrowser.GetWindowText(wfilePath);
	string filePath = common::wstr2str((wstring)wfilePath);
	filePath += "\\";
	UpdateTextureFiles(filePath);

}


// 지형 레이어 리스트를 업데이트 한다.
void CBrushPanel::UpdateLayerList()
{
	m_layerList.DeleteAllItems();

	graphic::cTerrainEditor &terrain = cMapController::Get()->GetTerrain();
	for (int i=0; i < terrain.GetSplatLayerCount(); ++i)
	{
		wstring layer = common::formatw( "%d", i + 1 );
		wstring texture = str2wstr(terrain.GetSplatLayer(i).texture->GetTextureName());
		m_layerList.InsertItem( i, layer.c_str() );
		m_layerList.SetItemText(i, 1, texture.c_str());
	}

}


void CBrushPanel::OnNMCustomdrawSliderInnerRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	m_innerRadius = m_innerRSlider.GetPos();
	cMapController::Get()->GetTerrainCursor().SetInnerBrushRadius(m_innerRadius);
	cMapController::Get()->UpdateBrush();
	UpdateData(FALSE);
	*pResult = 0;
}


void CBrushPanel::OnNMCustomdrawSliderOuterRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	m_outerRadius = m_outerRSlider.GetPos();
	cMapController::Get()->GetTerrainCursor().SetOuterBrushRadius(m_outerRadius);
	cMapController::Get()->UpdateBrush();
	UpdateData(FALSE);
	*pResult = 0;
}


void CBrushPanel::OnEnChangeEditInnerRadius()
{
	UpdateData();
	m_innerRSlider.SetPos(m_innerRadius);
	cMapController::Get()->GetTerrainCursor().SetInnerBrushRadius(m_innerRadius);
	cMapController::Get()->UpdateBrush();
}


void CBrushPanel::OnEnChangeEditOuterRadius2()
{
	UpdateData();
	m_outerRSlider.SetPos(m_outerRadius);
	cMapController::Get()->GetTerrainCursor().SetOuterBrushRadius(m_outerRadius);
	cMapController::Get()->UpdateBrush();
}
