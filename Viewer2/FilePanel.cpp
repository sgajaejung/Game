// FilePanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "FilePanel.h"
#include "afxdialogex.h"


// CFilePanel 대화 상자입니다.
CFilePanel::CFilePanel(CWnd* pParent /*=NULL*/)
	: CPanelBase(CFilePanel::IDD, pParent)
	, m_textModelFile(_T(""))
	, m_textAnimationFiles(_T(""))
{

}

CFilePanel::~CFilePanel()
{
}

void CFilePanel::DoDataExchange(CDataExchange* pDX)
{
	CPanelBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODEL, m_ModelFileList);
	DDX_Control(pDX, IDC_LIST_ANIMATION, m_AnimationFileList);
	DDX_Text(pDX, IDC_STATIC_MODEL_FILES, m_textModelFile);
	DDX_Text(pDX, IDC_STATIC_ANIMATION_FILES, m_textAnimationFiles);
}


BEGIN_MESSAGE_MAP(CFilePanel, CPanelBase)
	ON_BN_CLICKED(IDOK, &CFilePanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFilePanel::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_LBN_DBLCLK(IDC_LIST_ANIMATION, &CFilePanel::OnDblclkListAnimation)
	ON_LBN_DBLCLK(IDC_LIST_MODEL, &CFilePanel::OnDblclkListModel)
END_MESSAGE_MAP()


BOOL CFilePanel::OnInitDialog()
{
	__super::OnInitDialog();

	// ../../media  폴더에서 *.dat, *.ani 파일을 찾아서 리스팅한다.
	UpdateModelFiles();
	UpdateAnimationFiles();

	return TRUE; 
}


void CFilePanel::Update()
{
	// 아직 하는 일 없음.
}


// CFilePanel 메시지 처리기입니다.
void CFilePanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFilePanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// ../media  폴더에서 *.dat 파일을 찾아서 리스팅한다.
void CFilePanel::UpdateModelFiles()
{
	// 리스트 박스 초기화.
	while (0 < m_ModelFileList.GetCount())
		m_ModelFileList.DeleteString(0);

	// 파일 찾기.
	list<string> extList;
	extList.push_back("dat");
	list<string> modelFiles;
	common::CollectFiles(extList, "../media/", modelFiles);

	BOOST_FOREACH(auto &fileName, modelFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_ModelFileList.InsertString(m_ModelFileList.GetCount(), wstr.c_str());
	}

	m_textModelFile.Format( L"Model Files : %d", modelFiles.size());
	UpdateData(FALSE);
}


// ../media  폴더에서 *.ani 파일을 찾아서 리스팅한다.
void CFilePanel::UpdateAnimationFiles()
{
	// 리스트 박스 초기화.
	while (0 < m_AnimationFileList.GetCount())
		m_AnimationFileList.DeleteString(0);

	// 파일 찾기.
	list<string> extList;
	extList.push_back("ani");
	list<string> aniFiles;
	common::CollectFiles(extList, "../media/", aniFiles);

	BOOST_FOREACH(auto &fileName, aniFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_AnimationFileList.InsertString(m_AnimationFileList.GetCount(), wstr.c_str());
	}

	m_textAnimationFiles.Format( L"Animation Files : %d", aniFiles.size());
	UpdateData(FALSE);
}



void CFilePanel::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	MoveChildCtrlWindow(m_ModelFileList, cx, cy);
	MoveChildCtrlWindow(m_AnimationFileList, cx, cy);	
}


// 애니메이션 파일을 연다.
void CFilePanel::OnDblclkListAnimation()
{
	const int selIdx = m_AnimationFileList.GetCurSel();
	if (selIdx < 0)
		return;

	CString text;
	m_AnimationFileList.GetText(selIdx, text);
	const string fileName = wstr2str((wstring)text);
	cController::Get()->LoadFile(fileName);	
}


// 모델 파일을 연다.
void CFilePanel::OnDblclkListModel()
{
	const int selIdx = m_ModelFileList.GetCurSel();
	if (selIdx < 0)
		return;

	CString text;
	m_ModelFileList.GetText(selIdx, text);
	const string fileName = wstr2str((wstring)text);

	ShowLoadingDialog();
	cController::Get()->LoadFile(fileName);
}
