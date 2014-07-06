#pragma once
#include "afxcmn.h"


namespace graphic  {
	class cBoneNode;
}


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
	void UpdateBoneInfo();
	void UpdateRawBoneInfo();
	void MakeBoneTree(HTREEITEM hParent,  graphic::cBoneNode *node);
	void ExpandAll(CTreeCtrl &treeCtrl);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_MeshTree;
	CTreeCtrl m_MaterialTree;
	CTreeCtrl m_RawBoneTree;
	CTreeCtrl m_BoneTree;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
