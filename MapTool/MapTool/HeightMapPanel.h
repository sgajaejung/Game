#pragma once
#include "afxwin.h"


// CHeightMapPanel 대화 상자입니다.

class CHeightMapPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CHeightMapPanel)

public:
	CHeightMapPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHeightMapPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_HEIGHTMAP };

	
	void UpdateHeightMapList();


private:
	Image *m_heightMap;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_HeightMapList;
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListHeightmap();
	afx_msg void OnSelchangeListHeightmap();
	afx_msg void OnPaint();
};
