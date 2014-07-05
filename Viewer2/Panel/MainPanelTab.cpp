// MainPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../Viewer2.h"
#include "MainPanelTab.h"
#include "afxdialogex.h"
#include "ModelPanel.h"


// CMainPanelDlg 대화 상자입니다.

CMainPanelTab::CMainPanelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanelTab::IDD, pParent)
,	m_pModelPanel(NULL)
{

}

CMainPanelTab::~CMainPanelTab()
{
}

void CMainPanelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PANEL, m_Tab);
}


BEGIN_MESSAGE_MAP(CMainPanelTab, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainPanelTab::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainPanelTab::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PANEL, &CMainPanelTab::OnSelchangeTab)
END_MESSAGE_MAP()


// CMainPanelDlg 메시지 처리기입니다.


void CMainPanelTab::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CMainPanelTab::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


BOOL CMainPanelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Tab.InsertItem(0, L"Model");
	m_Tab.InsertItem(1, L"Animation");

	CRect cr;
	GetClientRect(cr);

	m_pModelPanel = new CModelPanel(this);
	m_pModelPanel->Create(CModelPanel::IDD, this);
	m_pModelPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_pModelPanel->ShowWindow(SW_SHOW);

	return TRUE;
}


void CMainPanelTab::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_Tab.GetCurSel())
	{
	case 0:
		m_pModelPanel->ShowWindow(SW_SHOW);
		break;

	case 1:
		m_pModelPanel->ShowWindow(SW_HIDE);
		break;
	}

	*pResult = 0;
}
