// MainPanelTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "MainPanelTab.h"
#include "afxdialogex.h"
#include "ModelPanel.h"
#include "AnimationPanel.h"
#include "FilePanel.h"
#include "ArchebladePanel.h"



// CMainPanelTab 대화 상자입니다.

CMainPanelTab::CMainPanelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanelTab::IDD, pParent)
,	m_modelPanel(NULL)
,	m_aniPanel(NULL)
,	m_filePanel(NULL)
,	m_archePanel(NULL)
{

}

CMainPanelTab::~CMainPanelTab()
{
	SAFE_DELETE(m_modelPanel);
	SAFE_DELETE(m_aniPanel);
	SAFE_DELETE(m_filePanel);
	SAFE_DELETE(m_archePanel);
}

void CMainPanelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CMainPanelTab, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainPanelTab::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainPanelTab::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainPanelTab::OnSelchangeTab)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainPanelTab 메시지 처리기입니다.


void CMainPanelTab::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CMainPanelTab::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


BOOL CMainPanelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Tab.InsertItem(0, L"Model File");
	m_Tab.InsertItem(1, L"Model");
	m_Tab.InsertItem(2, L"Animation");
	m_Tab.InsertItem(3, L"ArcheBlade");

	CRect cr;
	GetClientRect(cr);

	m_modelPanel = new CModelPanel(this);
	m_modelPanel->Create(CModelPanel::IDD, this);
	m_modelPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_modelPanel->ShowWindow(SW_HIDE);

	m_aniPanel = new CAnimationPanel(this);
	m_aniPanel->Create(CAnimationPanel::IDD, this);
	m_aniPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_aniPanel->ShowWindow(SW_HIDE);

	m_filePanel = new CFilePanel(this);
	m_filePanel->Create(CFilePanel::IDD, this);
	m_filePanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_filePanel->ShowWindow(SW_SHOW);

	m_archePanel = new CArchebladePanel(this);
	m_archePanel->Create(CArchebladePanel::IDD, this);
	m_archePanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_archePanel->ShowWindow(SW_HIDE);


	cController::Get()->AddObserver(m_modelPanel);
	cController::Get()->AddObserver(m_aniPanel);
	cController::Get()->AddObserver(m_filePanel);
	cController::Get()->AddObserver(m_archePanel);

	return TRUE;
}


void CMainPanelTab::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_Tab.GetCurSel())
	{
	case 0:
		m_aniPanel->ShowWindow(SW_HIDE);
		m_modelPanel->ShowWindow(SW_HIDE);
		m_filePanel->ShowWindow(SW_SHOW);
		m_archePanel->ShowWindow(SW_HIDE);
		break;

	case 1:
		m_modelPanel->ShowWindow(SW_SHOW);
		m_aniPanel->ShowWindow(SW_HIDE);
		m_filePanel->ShowWindow(SW_HIDE);
		m_archePanel->ShowWindow(SW_HIDE);
		break;

	case 2:
		m_aniPanel->ShowWindow(SW_SHOW);
		m_modelPanel->ShowWindow(SW_HIDE);
		m_filePanel->ShowWindow(SW_HIDE);
		m_archePanel->ShowWindow(SW_HIDE);
		break;

	case 3:
		m_archePanel->ShowWindow(SW_SHOW);
		m_aniPanel->ShowWindow(SW_HIDE);
		m_modelPanel->ShowWindow(SW_HIDE);
		m_filePanel->ShowWindow(SW_HIDE);
		break;
	}

	*pResult = 0;
}


void CMainPanelTab::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_Tab.GetSafeHwnd())
	{
		m_Tab.MoveWindow(0, 0, cx, cy);
		m_modelPanel->MoveWindow(CRect(0, 25, cx, cy));
		m_aniPanel->MoveWindow(CRect(0, 25, cx, cy));
		m_filePanel->MoveWindow(CRect(0, 25, cx, cy));
		m_archePanel->MoveWindow(CRect(0, 25, cx, cy));
	}
}
