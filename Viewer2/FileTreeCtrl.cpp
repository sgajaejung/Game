// FileTreeCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "FileTreeCtrl.h"


CImageList *CFileTreeCtrl::m_imageList = NULL;
int CFileTreeCtrl::m_imageListRefCnt = 0;

// CFileTreeCtrl
CFileTreeCtrl::CFileTreeCtrl()
{
	if (!m_imageList)
	{
		SHFILEINFO sfi = { 0 }; 
		m_imageList = new CImageList();
		m_imageList->Attach((HIMAGELIST)SHGetFileInfo(L"C:\\", 0, &sfi, sizeof(sfi), SHGFI_SMALLICON | SHGFI_SYSICONINDEX));
	}

	++m_imageListRefCnt;

}

CFileTreeCtrl::~CFileTreeCtrl()
{
	--m_imageListRefCnt;
	if (m_imageListRefCnt <= 0)
	{
		SAFE_DELETE(m_imageList);
		m_imageListRefCnt = 0;
	}
}


BEGIN_MESSAGE_MAP(CFileTreeCtrl, CTreeCtrl)
END_MESSAGE_MAP()



// CFileTreeCtrl 메시지 처리기입니다.

// fileList에 저장된 파일 경로 스트링으로 트리를 만든다.
void CFileTreeCtrl::Update(const list<string> &fileList)
{
	DeleteAllItems();	

	SetImageList(m_imageList, TVSIL_NORMAL);

	sTreeNode *rootNode = GenerateTreeNode(fileList);
	GenerateTreeItem(NULL, rootNode);	
	Expand(GetRootItem(), TVE_EXPAND);

	DeleteTreeNode(rootNode);

	m_fileCount = (int)fileList.size();
}


void CFileTreeCtrl::Update(const string &directoryPath, const list<string> &extList)
{
	list<string> files;
	common::CollectFiles(extList, directoryPath, files);
	Update(files);
}


// sTreeNode 생성한다.
CFileTreeCtrl::sTreeNode* CFileTreeCtrl::GenerateTreeNode(const list<string> &fileList)
{
	sTreeNode *rootNode = new sTreeNode;

	BOOST_FOREACH (auto &str, fileList)
	{
		vector<string> strs;
		common::tokenizer( str, "/", "..", strs);

		sTreeNode *node = rootNode;
		BOOST_FOREACH (auto &name, strs)
		{
			auto it = node->children.find(name);
			if (node->children.end() ==  it)
			{
				sTreeNode *t = new sTreeNode;
				node->children[ name] = t;
				node = t;
			}
			else
			{
				node = it->second;
			}
		}	
	}

	return rootNode;
}


// treenode 제거.
void CFileTreeCtrl::DeleteTreeNode(sTreeNode *node)
{
	RET(!node);
	BOOST_FOREACH (auto &child, node->children)
		DeleteTreeNode(child.second);
	delete node;
}


// sTreeNode 정보로 TreeCtrl 노드를 생성한다.
void CFileTreeCtrl::GenerateTreeItem(HTREEITEM parent, sTreeNode*node)
{
	RET(!node);

	BOOST_FOREACH (auto &child, node->children)
	{
		const int imageIdx = child.second->children.empty()? 0 : 1;
		HTREEITEM item = InsertItem(str2wstr(child.first).c_str(), imageIdx, imageIdx, parent);
		GenerateTreeItem(item, child.second);
	}
}


// 선택된 트리노드의 파일 경로를 리턴한다.
string CFileTreeCtrl::GetSelectFilePath(HTREEITEM item)
{
	wstring path;
	HTREEITEM curItem = item;
	while (curItem)
	{
		if (path.empty())
			path = (wstring)GetItemText(curItem);
		else
			path = (wstring)GetItemText(curItem) + "/" + path;

		curItem = GetParentItem(curItem);
	}

	return wstr2str(path);
}
