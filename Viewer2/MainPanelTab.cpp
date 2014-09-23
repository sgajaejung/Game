// MainPanelTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "MainPanelTab.h"
#include "ModelPanel.h"
#include "AnimationPanel.h"
#include "FilePanel.h"
#include "ArchebladePanel.h"
#include "TeraPanel.h"
#include "LightPanel.h"


// CMainPanelTab 대화 상자입니다.

CMainPanelTab::CMainPanelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanelTab::IDD, pParent)
,	m_modelPanel(NULL)
,	m_aniPanel(NULL)
,	m_filePanel(NULL)
,	m_archePanel(NULL)
,	m_teraPanel(NULL)
{

}

CMainPanelTab::~CMainPanelTab()
{
	SAFE_DELETE(m_modelPanel);
	SAFE_DELETE(m_aniPanel);
	SAFE_DELETE(m_filePanel);
	SAFE_DELETE(m_archePanel);
	SAFE_DELETE(m_teraPanel);
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
	m_Tab.InsertItem(3, L"Light");
	m_Tab.InsertItem(4, L"ArcheBlade");
	m_Tab.InsertItem(5, L"Tera");

	CRect cr;
	GetClientRect(cr);

	m_filePanel = new CFilePanel(this);
	m_filePanel->Create(CFilePanel::IDD, this);
	m_filePanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_panels.push_back(m_filePanel);

	m_modelPanel = new CModelPanel(this);
	m_modelPanel->Create(CModelPanel::IDD, this);
	m_modelPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_panels.push_back(m_modelPanel);

	m_aniPanel = new CAnimationPanel(this);
	m_aniPanel->Create(CAnimationPanel::IDD, this);
	m_aniPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_panels.push_back(m_aniPanel);

	m_lightPanel = new CLightPanel(this);
	m_lightPanel->Create(CLightPanel::IDD, this);
	m_lightPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_panels.push_back(m_lightPanel);

	m_archePanel = new CArchebladePanel(this);
	m_archePanel->Create(CArchebladePanel::IDD, this);
	m_archePanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_panels.push_back(m_archePanel);

	m_teraPanel = new CTeraPanel(this);
	m_teraPanel->Create(CTeraPanel::IDD, this);
	m_teraPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_panels.push_back(m_teraPanel);

	BOOST_FOREACH (auto &panel, m_panels)
	{
		if (iObserver *observer = dynamic_cast<iObserver*>(panel))
			cController::Get()->AddObserver(observer);
	}

	ShowPanel(0);

	return TRUE;
}


void CMainPanelTab::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShowPanel(m_Tab.GetCurSel());
	cController::Get()->ChangePanel(m_Tab.GetCurSel());
	*pResult = 0;
}


void CMainPanelTab::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_Tab.GetSafeHwnd())
	{
		m_Tab.MoveWindow(0, 0, cx, cy);

		BOOST_FOREACH (auto &panel, m_panels)
			panel->MoveWindow(CRect(0, 25, cx, cy));
	}
}


void CMainPanelTab::ShowPanel(int idx)
{
	BOOST_FOREACH (auto &panel, m_panels)
		panel->ShowWindow(SW_HIDE);

	if ((int)m_panels.size() > idx)
		m_panels[ idx]->ShowWindow(SW_SHOW);
}
