#pragma once
#include "afxeditbrowsectrl.h"
#include "afxwin.h"
#include "afxcmn.h"


// CModelPanel 대화 상자입니다.

class CModelPanel : public CDialogEx
								, public iObserver2
{
public:
	CModelPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CModelPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL };


public:
	virtual void Update(int type) override;
	void UpdateModelList(const string &directoryPath);
	void UpdatePlaceModelList();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CMFCEditBrowseCtrl m_modelBrowser;
	CListBox m_modelList;
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeMfceditbrowseModel();
	afx_msg void OnSelchangeListModel();
	CListCtrl m_placeModelList;
};
