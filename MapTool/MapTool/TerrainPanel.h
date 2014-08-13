#pragma once
#include "afxeditbrowsectrl.h"


// CTerrainPanel 대화 상자입니다.

class CTerrainPanel : public CDialogEx
								, public common::iObserver2
{
public:
	CTerrainPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTerrainPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TERRAIN };


public:
	virtual void Update(int type) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonLoadTerrain();
	afx_msg void OnBnClickedButtonCrterrain();
	afx_msg void OnBnClickedButtonSaveTerrain();
	int m_colCellCount;
	int m_rowCellCount;
	float m_cellSize;
	CString m_textureName;
	CMFCEditBrowseCtrl m_textureBrowser;
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeMfceditbrowseTexture();
};
