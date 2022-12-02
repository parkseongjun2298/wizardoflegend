#pragma once
#include "afxwin.h"


// CPathTool ��ȭ �����Դϴ�.

class CPathTool : public CDialog
{
	DECLARE_DYNAMIC(CPathTool)

public:
	CPathTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private: // User Functions
	void SetHorizontalScrollSizes();

public:	// Message Functions
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:	// Control Variables
	CListBox m_ListBox;

public: // User Variables
	list<PATH_INFO*>	m_PathInfoList;		
};
