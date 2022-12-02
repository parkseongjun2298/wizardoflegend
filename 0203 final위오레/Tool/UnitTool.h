#pragma once
#include "afxwin.h"


// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:	// Virtual Functions
	virtual BOOL OnInitDialog();

public:	// Message Functions
	afx_msg void OnBnClickedPush();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnLbnSelectUnit();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnEnChangeFindName();
	afx_msg void OnBnClickedDelete();

public:	// Value variables
	CString m_strInput; // ù��° Edit Control���� ���ڿ��� �Է¹���.	
	CString m_strOutput; // �ι�° Edit Control�� ���ڿ��� �����.
						 
	CString m_strName; // ĳ���� �̸�	
	int m_iAtt; // ĳ���� ���ݷ�	
	int m_iDef; // ĳ���� ����
				
	CString m_strFindName; // �̸� �˻�

public:	// Control variables
	CListBox m_ListBox;
	CButton m_RadioBtn[3];
	CButton m_CheckBox[3];

public: // User variables
	map<wstring, UNIT_INFO*>	m_mapUnit;			
	
};
