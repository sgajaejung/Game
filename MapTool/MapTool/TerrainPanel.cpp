// TerrainPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainPanel.h"
#include "afxdialogex.h"


// CTerrainPanel 대화 상자입니다.
CTerrainPanel::CTerrainPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTerrainPanel::IDD, pParent)
	, m_colCellCount(0)
	, m_rowCellCount(0)
	, m_cellSize(0)
{

}

CTerrainPanel::~CTerrainPanel()
{
}

void CTerrainPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_COL_CELLCNT, m_colCellCount);
	DDX_Text(pDX, IDC_STATIC_ROW_CELLCNT, m_rowCellCount);
	DDX_Text(pDX, IDC_STATIC_CELLSIZE, m_cellSize);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_TEXTURE, m_textureBrowser);
}


BEGIN_MESSAGE_MAP(CTerrainPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTerrainPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTerrainPanel::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TERRAIN, &CTerrainPanel::OnBnClickedButtonLoadTerrain)
	ON_BN_CLICKED(IDC_BUTTON_CRTERRAIN, &CTerrainPanel::OnBnClickedButtonCrterrain)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TERRAIN, &CTerrainPanel::OnBnClickedButtonSaveTerrain)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_TEXTURE, &CTerrainPanel::OnChangeMfceditbrowseTexture)
END_MESSAGE_MAP()


// CTerrainPanel 메시지 처리기입니다.
BOOL CTerrainPanel::OnInitDialog()
{
	__super::OnInitDialog();

	m_textureBrowser.EnableFileBrowseButton(L"Texture", L"All files(*.*) |*.*||");
	return TRUE;
}


// MapController 가 업데이트 될 때 호출된다.
void CTerrainPanel::Update(int type)
{
	switch (type)
	{
	case NOTIFY_TYPE::NOTIFY_CHANGE_TERRAIN:
		{
			m_colCellCount = cMapController::Get()->GetTerrain().GetColCellCount();
			m_rowCellCount = cMapController::Get()->GetTerrain().GetRowCellCount();
			m_cellSize = cMapController::Get()->GetTerrain().GetCellSize();
			const wstring wstr = common::str2wstr(cMapController::Get()->GetTerrain().GetTextureName());
			m_textureBrowser.SetWindowText(wstr.c_str());

			UpdateData(FALSE);
		}
		break;
	}
}


void CTerrainPanel::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CTerrainPanel::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


void CTerrainPanel::OnBnClickedButtonLoadTerrain()
{
	// 파일 열기 창 오픈
	wchar_t szFilter[] = L"Terrain (*.trn) | *.trn; | All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, L"trn", L"Terrain", OFN_HIDEREADONLY, szFilter);
	if(IDOK == dlg.DoModal())
	{
		const CString strPathName = dlg.GetPathName();
		const string fileName = wstr2str((wstring)strPathName);
		cMapController::Get()->LoadTerrainFile(fileName);
	}
}


void CTerrainPanel::OnBnClickedButtonCrterrain()
{
	cMapController::Get()->CreateDefaultTerrain();

}


void CTerrainPanel::OnBnClickedButtonSaveTerrain()
{
	// 파일 저장 창 오픈
	wchar_t szFilter[] = L"Terrain (*.trn) | *.trn; | All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE, L"trn", L"Terrain", OFN_HIDEREADONLY, szFilter);
	if(IDOK == dlg.DoModal())
	{
		const CString strPathName = dlg.GetPathName();
		const string fileName = wstr2str((wstring)strPathName);
		cMapController::Get()->SaveTerrainFile(fileName);
	}
}


void CTerrainPanel::OnChangeMfceditbrowseTexture()
{
	CString fileName;
	m_textureBrowser.GetWindowText(fileName);

	// 파일명이 바꼈다면 업데이트.
	const wstring wstr = common::str2wstr(cMapController::Get()->GetTerrain().GetTextureName());
	if (fileName != wstr.c_str())
	{
		const string strFileName = wstr2str((wstring)fileName);
		cMapController::Get()->GetTerrain().CreateTerrainTexture(strFileName);

		UpdateData(FALSE);
	}
}
