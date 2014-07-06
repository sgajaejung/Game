// ModelPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../Viewer2.h"
#include "ModelPanel.h"
#include "afxdialogex.h"

using namespace graphic;


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
	DDX_Control(pDX, IDC_TREE_MESH, m_MeshTree);
	DDX_Control(pDX, IDC_TREE_MATERIAL, m_MaterialTree);
}


BEGIN_MESSAGE_MAP(CModelPanel, CDialogEx)
END_MESSAGE_MAP()


// CModelPanel 메시지 처리기입니다.

void CModelPanel::Update()
{
	UpdateMaterialInfo();
	UpdateMeshInfo();	



}


void CModelPanel::UpdateMeshInfo()
{
	m_MeshTree.DeleteAllItems();



}


void CModelPanel::UpdateMaterialInfo()
{
	m_MaterialTree.DeleteAllItems();

	const sRawMeshGroup *rawMeshses = cResourceManager::Get()->LoadModel( 
		cController::Get()->GetCurrentMeshFileName() );
	RET (!rawMeshses);


	const HTREEITEM hRoot = m_MaterialTree.InsertItem(L"Material");

	for (u_int i=0; i < rawMeshses->mtrls.size(); ++i )
	{
		const sMaterial &mtrl = rawMeshses->mtrls[ i];
		const wstring str = formatw("Material %d", i+1);		
		const HTREEITEM hItem = m_MaterialTree.InsertItem( str.c_str(), hRoot);

		const wstring ambient = formatw("Ambient %.3f, %.3f, %.3f, %.3f", 
			mtrl.ambient.x, mtrl.ambient.y, mtrl.ambient.z, mtrl.ambient.w );
		const wstring diffuse = formatw("Diffuse %.3f, %.3f, %.3f, %.3f", 
			mtrl.diffuse.x, mtrl.diffuse.y, mtrl.diffuse.z, mtrl.diffuse.w );
		const wstring specular = formatw("Specular %.3f, %.3f, %.3f, %.3f", 
			mtrl.specular.x, mtrl.specular.y, mtrl.specular.z, mtrl.specular.w );
		const wstring emissive = formatw("Emissive %.3f, %.3f, %.3f, %.3f", 
			mtrl.emissive.x, mtrl.emissive.y, mtrl.emissive.z, mtrl.emissive.w );
		const wstring power = formatw("Power = %.3f", mtrl.power);
		const wstring texture = formatw("texture = %s", mtrl.texture.c_str());

		m_MaterialTree.InsertItem( ambient.c_str(), hItem);
		m_MaterialTree.InsertItem( diffuse.c_str(), hItem);
		m_MaterialTree.InsertItem( specular.c_str(), hItem);
		m_MaterialTree.InsertItem( emissive.c_str(), hItem);
		m_MaterialTree.InsertItem( power.c_str(), hItem);
		m_MaterialTree.InsertItem( texture.c_str(), hItem);
	}

	m_MaterialTree.Expand(hRoot, TVE_EXPAND);
}
