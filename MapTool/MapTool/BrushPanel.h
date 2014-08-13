#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"


// CBrushPanel 대화 상자입니다.

class CBrushPanel : public CDialogEx
								, public common::iObserver
{
public:
	CBrushPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBrushPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BRUSH };


public:
	virtual void Update() override;


protected:
	void UpdateTextureFiles( const string &directoryPath );
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	Image *m_texture;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_TextureFiles;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListTextureFiles();
	afx_msg void OnPaint();
	CMFCEditBrowseCtrl m_textureBrowser;
	afx_msg void OnChangeMfceditbrowseTexture();
	CListCtrl m_layerList;
};
