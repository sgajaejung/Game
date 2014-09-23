// LightPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "LightPanel.h"
#include "afxdialogex.h"


// CLightPanel 대화 상자입니다.
CLightPanel::CLightPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightPanel::IDD, pParent)
{

}

CLightPanel::~CLightPanel()
{
}

void CLightPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLightPanel, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CLightPanel::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CLightPanel::OnBnClickedOk)
END_MESSAGE_MAP()


// CLightPanel 메시지 처리기입니다.


void CLightPanel::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


void CLightPanel::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}
