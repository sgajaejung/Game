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
	, m_Radius(0)
	, m_Speed(0)
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
	DDX_Control(pDX, IDC_COMBO_TERRAIN_EDIT, m_ComboEditMode);
	DDX_Control(pDX, IDC_SLIDER_OUTER_RADIUS, m_SliderRadius);
	DDX_Text(pDX, IDC_EDIT_OUTER_RADIUS2, m_Radius);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_SliderSpeed);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_Speed);
}


BEGIN_MESSAGE_MAP(CTerrainPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTerrainPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTerrainPanel::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TERRAIN, &CTerrainPanel::OnBnClickedButtonLoadTerrain)
	ON_BN_CLICKED(IDC_BUTTON_CRTERRAIN, &CTerrainPanel::OnBnClickedButtonCrterrain)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TERRAIN, &CTerrainPanel::OnBnClickedButtonSaveTerrain)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_TEXTURE, &CTerrainPanel::OnChangeMfceditbrowseTexture)
	ON_CBN_SELCHANGE(IDC_COMBO_TERRAIN_EDIT, &CTerrainPanel::OnCbnSelchangeComboTerrainEdit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_OUTER_RADIUS, &CTerrainPanel::OnNMCustomdrawSliderOuterRadius)
	ON_EN_CHANGE(IDC_EDIT_OUTER_RADIUS2, &CTerrainPanel::OnEnChangeEditOuterRadius2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SPEED, &CTerrainPanel::OnNMCustomdrawSliderSpeed)
	ON_EN_CHANGE(IDC_EDIT_SPEED, &CTerrainPanel::OnEnChangeEditSpeed)
END_MESSAGE_MAP()


// CTerrainPanel 메시지 처리기입니다.
BOOL CTerrainPanel::OnInitDialog()
{
	__super::OnInitDialog();

	m_textureBrowser.EnableFileBrowseButton(L"Texture", L"All files(*.*) |*.*||");

	m_ComboEditMode.AddString( L"None" );
	m_ComboEditMode.AddString( L"Up" );
	m_ComboEditMode.AddString( L"Down" );
	m_ComboEditMode.SetCurSel(0);

	m_Speed = cMapController::Get()->GetTerrainCursor().GetBrushSpeed();
	m_SliderSpeed.SetRange(10, 300);
	m_SliderSpeed.SetPos(m_Speed);

	m_Radius = cMapController::Get()->GetTerrainCursor().GetOuterBrushRadius();
	m_SliderRadius.SetRange(0, 300);
	m_SliderRadius.SetPos(m_Radius);

	UpdateData(FALSE);
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
	if (cMapController::Get()->GetTerrain().IsLoaded())
	{
		if (IDYES == ::AfxMessageBox(L"이미 열려있는 지형 정보가 있습니다. 저장하시겠습니까?", 
			MB_YESNO))
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
			else
			{
				// 파일 저장 실패, 종료.
			}			
			
		}
	}

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


void CTerrainPanel::OnCbnSelchangeComboTerrainEdit()
{
	const int idx = m_ComboEditMode.GetCurSel();
	
	cMapController::Get()->GetTerrainCursor().SetTerrainEditMode((graphic::TERRAIN_EDIT_MODE::TYPE)idx);
	cMapController::Get()->SendNotifyMessage( NOTIFY_TYPE::NOTIFY_CHANGE_TERRAIN_EDIT_MODE );
}


void CTerrainPanel::OnNMCustomdrawSliderOuterRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	m_Radius = m_SliderRadius.GetPos();
	cMapController::Get()->GetTerrainCursor().SetOuterBrushRadius(m_Radius);
	cMapController::Get()->UpdateBrush();
	UpdateData(FALSE);
	*pResult = 0;
}


void CTerrainPanel::OnEnChangeEditOuterRadius2()
{
	UpdateData();
	m_SliderRadius.SetPos(m_Radius);
	cMapController::Get()->GetTerrainCursor().SetOuterBrushRadius(m_Radius);
	cMapController::Get()->UpdateBrush();
}


void CTerrainPanel::OnNMCustomdrawSliderSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	m_Speed = m_SliderSpeed.GetPos();
	cMapController::Get()->GetTerrainCursor().SetBrushSpeed(m_Speed);
	UpdateData(FALSE);
	*pResult = 0;
}


void CTerrainPanel::OnEnChangeEditSpeed()
{
	UpdateData();
	if ((m_Speed < 10) || (m_Speed > 300))
		return;

	m_SliderSpeed.SetPos(m_Speed);
	cMapController::Get()->GetTerrainCursor().SetBrushSpeed(m_Speed);
}
