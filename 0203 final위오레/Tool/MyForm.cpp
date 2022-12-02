// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM),
	m_MySheet(L"MySheet")
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedUnitTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnBnClickedPathTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnBnClickedMySheet)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	// MFC ��Ʈ ����
	m_Font.CreatePointFont(180, L"�ü�");

	// ��ȭ���ڿ� ��ġ�Ǿ��ִ� ������Ʈ�� ������ �Լ�.
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);

	m_UnitTool.Create(IDD_UNITTOOL);
	m_MapTool.Create(IDD_MAPTOOL);
	m_PathTool.Create(IDD_PATHTOOL);
	m_MySheet.Create(this, WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXTHELP);
}


void CMyForm::OnBnClickedUnitTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//AfxMessageBox(L"UnitTool ��ư�� Ŭ���߽��ϴ�");

	if (nullptr != m_UnitTool.GetSafeHwnd())
	{
		// SW_SHOW: â������� ���.
		m_UnitTool.ShowWindow(SW_SHOW);
	}	
}


void CMyForm::OnBnClickedMapTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr != m_MapTool.GetSafeHwnd())
	{
		// SW_SHOW: â������� ���.
		m_MapTool.ShowWindow(SW_SHOW);
	}
}


void CMyForm::OnBnClickedPathTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr != m_PathTool.GetSafeHwnd())
	{
		// SW_SHOW: â������� ���.
		m_PathTool.ShowWindow(SW_SHOW);
	}
}


void CMyForm::OnBnClickedMySheet()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr != m_MySheet.GetSafeHwnd())
	{
		// SW_SHOW: â������� ���.
		m_MySheet.ShowWindow(SW_SHOW);
	}
}
