// ModelPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "ModelPanel.h"
#include "afxdialogex.h"


// CModelPanel 대화 상자입니다.
CModelPanel::CModelPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModelPanel::IDD, pParent)
{

}

CModelPanel::~CModelPanel()
{
}

void CModelPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_MODEL, m_modelBrowser);
	DDX_Control(pDX, IDC_LIST_MODEL, m_modelList);
	DDX_Control(pDX, IDC_LIST_PLACE_MODEL, m_placeModelList);
}


BEGIN_MESSAGE_MAP(CModelPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModelPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CModelPanel::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_MODEL, &CModelPanel::OnChangeMfceditbrowseModel)
	ON_LBN_SELCHANGE(IDC_LIST_MODEL, &CModelPanel::OnSelchangeListModel)
END_MESSAGE_MAP()


// CModelPanel 메시지 처리기입니다.


void CModelPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CModelPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


BOOL CModelPanel::OnInitDialog()
{
	__super::OnInitDialog();

	m_placeModelList.InsertColumn(0, L"num", LVCFMT_LEFT, 35);
	m_placeModelList.InsertColumn(1, L"model", LVCFMT_LEFT, 300);
	m_placeModelList.SetExtendedStyle(m_placeModelList.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT);

	m_modelBrowser.EnableFolderBrowseButton();
	m_modelBrowser.SetWindowText( L"../../media/" );
	UpdateModelList("../../media/" );

	return TRUE;
}


void CModelPanel::Update(int type)
{
	switch (type)
	{
	case NOTIFY_TYPE::NOTIFY_ADD_PLACE_MODEL:
		UpdatePlaceModelList();
		break;
	}

}


void CModelPanel::UpdateModelList(const string &directoryPath)
{
	// 리스트 박스 초기화.
	while (0 < m_modelList.GetCount())
		m_modelList.DeleteString(0);

	// 파일 찾기.
	list<string> extList;
	extList.push_back("dat");
	list<string> textureFiles;
	common::CollectFiles(extList, directoryPath, textureFiles);

	BOOST_FOREACH(auto &fileName, textureFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_modelList.InsertString(m_modelList.GetCount(), wstr.c_str());
	}	
}


void CModelPanel::OnChangeMfceditbrowseModel()
{
	CString wfilePath;
	m_modelBrowser.GetWindowText(wfilePath);
	string filePath = common::wstr2str((wstring)wfilePath);
	filePath += "\\";
	UpdateModelList(filePath);
}


void CModelPanel::OnSelchangeListModel()
{
	const int idx = m_modelList.GetCurSel();
	RET (idx < 0);

	CString wfileName;
	m_modelList.GetText(idx, wfileName);
	const string fileName = wstr2str((wstring)wfileName);
	cMapController::Get()->GetTerrainCursor().SelectModel( fileName);	
}


// 지형에 위치한 모델들을 리스트에 출력한다.
void CModelPanel::UpdatePlaceModelList()
{
	m_placeModelList.DeleteAllItems();

	vector<graphic::cModel*> &models = cMapController::Get()->GetTerrain().GetRigidModels();
	for (u_int i=0; i < models.size(); ++i)
	{
		const wstring num = common::formatw("%d", i+1);
		const wstring fileName = str2wstr( common::GetFileName(models[ i]->GetFileName()) );

		m_placeModelList.InsertItem(i, num.c_str());
		m_placeModelList.SetItemText(i, 1, fileName.c_str());
		m_placeModelList.SetItemData(i, models[ i]->GetId() );
	}
}
