#pragma once
#include "afxcmn.h"


// CModelPanel 대화 상자입니다.

class CModelPanel : public CDialogEx
								, public common::iObserver
{
public:
	CModelPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CModelPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL };

	virtual void Update() override;


protected:
	void UpdateMeshInfo();
	void UpdateMaterialInfo();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_MeshTree;
	CTreeCtrl m_MaterialTree;
};
