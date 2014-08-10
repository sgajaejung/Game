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
{

}

CBrushPanel::~CBrushPanel()
{
}

void CBrushPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEXTURE_FILES, m_TextureFiles);
}


BEGIN_MESSAGE_MAP(CBrushPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBrushPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBrushPanel::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_TEXTURE_FILES, &CBrushPanel::OnSelchangeListTextureFiles)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBrushPanel 메시지 처리기입니다.
BOOL CBrushPanel::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateTextureFiles();

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


void CBrushPanel::Update()
{

}


void CBrushPanel::UpdateTextureFiles()
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
	common::CollectFiles(extList, "../../media/terrain/", textureFiles);

	BOOST_FOREACH(auto &fileName, textureFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_TextureFiles.InsertString(m_TextureFiles.GetCount(), wstr.c_str());
	}
}


void CBrushPanel::OnSelchangeListTextureFiles()
{
	CString fileName;
	m_TextureFiles.GetText(m_TextureFiles.GetCurSel(), fileName);
	SAFE_DELETE(m_texture);
	m_texture = Image::FromFile(fileName);
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
