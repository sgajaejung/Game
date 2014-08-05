// TabPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TabPanel.h"
#include "afxdialogex.h"
#include "HeightMapPanel.h"


// CTabPanel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabPanel, CDialogEx)

CTabPanel::CTabPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPanel::IDD, pParent)
{

}

CTabPanel::~CTabPanel()
{
	SAFE_DELETE(m_HeightMapPanel);
}

void CTabPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CTabPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTabPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTabPanel::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabPanel::OnSelchangeTab1)
END_MESSAGE_MAP()


// CTabPanel 메시지 처리기입니다.


void CTabPanel::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CTabPanel::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


BOOL CTabPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Tab.InsertItem(0, L"HeightMap");
	m_Tab.InsertItem(1,  L"Terrain");

	CRect cr;
	GetClientRect(cr);

	m_HeightMapPanel = new CHeightMapPanel(this);
	m_HeightMapPanel->Create(CHeightMapPanel::IDD, this);
	m_HeightMapPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_HeightMapPanel->ShowWindow(SW_SHOW);

	return TRUE;
}


void CTabPanel::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_Tab.GetCurSel())
	{
	case 0:
		m_HeightMapPanel->ShowWindow(SW_SHOW);
		break;

	case 1:
		m_HeightMapPanel->ShowWindow(SW_HIDE);
		break;

	case 2:
		break;
	}

	*pResult = 0;
}
