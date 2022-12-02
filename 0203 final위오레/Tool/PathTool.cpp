// PathTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "afxdialogex.h"
#include "DirectoryMgr.h"


// CPathTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathTool, CDialog)

CPathTool::CPathTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHTOOL, pParent)
{

}

CPathTool::~CPathTool()
{
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), SafeDelete<PATH_INFO*>);
	m_PathInfoList.clear();
}

void CPathTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}

void CPathTool::SetHorizontalScrollSizes()
{
	CString strFileName = L"";
	CString strLonger = L"";
	int iLength = 0;

	// GetCount: ����Ʈ�ڽ� �ȿ� ����ִ� �������� ��ü ���� ��ȯ.
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strFileName);

		if (iLength < strFileName.GetLength())
		{
			strLonger = strFileName;
			iLength = strLonger.GetLength();
		}
	}

	// ���ڿ��� ���� �� �༮�� �������� ���� ��ũ�� ũ�⸦ �����Ѵ�.
	// ���ڿ��� ���̸� �ȼ� ������ ��ȯ.
	CDC* pDC = m_ListBox.GetDC();

	// GetTextExtent: ���� ���ڿ��� ũ�⸦ �ȼ������� ��ȯ�ؼ� CSize�� ��ȯ��.
	int iCX = pDC->GetTextExtent(strLonger).cx;

	// GetHorizontalExtent: ���� ���� ��ũ�� ����.
	if (iCX > m_ListBox.GetHorizontalExtent())
	{
		// ���� ��ũ�� ������ �����ϴ� �Լ�.
		m_ListBox.SetHorizontalExtent(iCX);
	}

	// GetDC�� �������� �ݵ�� ReleaseDC�� ��ȯ.
	m_ListBox.ReleaseDC(pDC);
}


BEGIN_MESSAGE_MAP(CPathTool, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CPathTool �޽��� ó�����Դϴ�.


void CPathTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialog::OnDropFiles(hDropInfo);

	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFilePath[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
		CDirectoryMgr::ExtractPathInfo(szFilePath, m_PathInfoList);
	}

	wstring wstrCombined = L"";
	TCHAR szFileCount[MIN_STR] = L"";

	for (auto& pPathInfo : m_PathInfoList)
	{
		// ���� -> ���� ��ȯ.
		_itow_s(pPathInfo->iFileCount, szFileCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
			L"|" + szFileCount + L"|" + pPathInfo->wstrRelativePath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	SetHorizontalScrollSizes();
}


void CPathTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, L".txt", L"���� ����.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files|*.txt|Data Files|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		// C++�� ���� ��� ������Ʈ
		wofstream fout;

		// C++�� ���� ����
		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szFileCount[MIN_STR] = L"";

		for (auto& pPathInfo : m_PathInfoList)
		{
			// ���� -> ���� ��ȯ.
			_itow_s(pPathInfo->iFileCount, szFileCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
				L"|" + szFileCount + L"|" + pPathInfo->wstrRelativePath;

			// C++�� �ؽ�Ʈ��� ���
			fout << wstrCombined << endl;

			// C++�� ���̳ʸ���� ���
			//fout.write()
		}

		fout.close();
	}

	// ���������� �⺻ ���� ���α׷� �����ϴ� �ý��� �Լ�.
	WinExec("notepad.exe ../Data/PathInfo.txt", SW_SHOW);
}


void CPathTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L".txt", L"���� ����.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files|*.txt|Data Files|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		if (!m_PathInfoList.empty())
		{
			for_each(m_PathInfoList.begin(), m_PathInfoList.end(), SafeDelete<PATH_INFO*>);
			m_PathInfoList.clear();

			m_ListBox.ResetContent();
		}

		// C++ ���� �Է�
		wifstream fin;

		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		TCHAR szObjectKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szCount[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";

		PATH_INFO* pPathInfo = nullptr;

		while (true)
		{
			fin.getline(szObjectKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_STR);

			// EOF�� ������ break!
			if (fin.eof())
				break;

			pPathInfo = new PATH_INFO;
			pPathInfo->wstrObjectKey = szObjectKey;
			pPathInfo->wstrStateKey = szStateKey;
			pPathInfo->iFileCount = _ttoi(szCount);
			pPathInfo->wstrRelativePath = szPath;

			m_PathInfoList.push_back(pPathInfo);
		}

		fin.close();
	}	

	wstring wstrCombined = L"";
	TCHAR szFileCount[MIN_STR] = L"";

	for (auto& pPathInfo : m_PathInfoList)
	{
		// ���� -> ���� ��ȯ.
		_itow_s(pPathInfo->iFileCount, szFileCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
			L"|" + szFileCount + L"|" + pPathInfo->wstrRelativePath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	SetHorizontalScrollSizes();
}
