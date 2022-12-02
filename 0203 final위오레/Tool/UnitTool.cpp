// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
	, m_strName(_T(""))
	, m_iAtt(0)
	, m_iDef(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	for (auto& MyPair : m_mapUnit)
		SafeDelete(MyPair.second);

	m_mapUnit.clear();
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInput);
	DDX_Text(pDX, IDC_EDIT2, m_strOutput);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAtt);
	DDX_Text(pDX, IDC_EDIT5, m_iDef);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_RadioBtn[0]);
	DDX_Control(pDX, IDC_RADIO2, m_RadioBtn[1]);
	DDX_Control(pDX, IDC_RADIO3, m_RadioBtn[2]);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox[0]);
	DDX_Control(pDX, IDC_CHECK2, m_CheckBox[1]);
	DDX_Control(pDX, IDC_CHECK3, m_CheckBox[2]);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}

BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_RadioBtn[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedPush)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelectUnit)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedLoad)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnEnChangeFindName)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnBnClickedDelete)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnBnClickedPush()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// UpdateData�Լ��� ȣ���ϸ� DoDataExchange�Լ��� ����ȴ�.
	// TRUE: ��Ʈ�� -> ������ ������ �̵�
	// FALSE: ���� -> ��Ʈ�ѷ� ������ �̵�
	UpdateData(TRUE);

	m_strOutput = m_strInput;

	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	auto iter_find = m_mapUnit.find(m_strName.GetString());

	if (m_mapUnit.end() != iter_find)
		return;

	UNIT_INFO* pUnit = new UNIT_INFO;
	pUnit->wstrName = m_strName.GetString();
	pUnit->iAtt = m_iAtt;
	pUnit->iDef = m_iDef;
	pUnit->iItem = 0;

	for (int i = 0; i < 3; ++i)
	{
		// ���� ��ư�� üũ�Ǿ� ������ TRUE ��ȯ.
		if (m_RadioBtn[i].GetCheck())
		{
			pUnit->iJobIndex = i;
			break;
		}
	}

	if (m_CheckBox[0].GetCheck())
		pUnit->iItem |= RUBY;
	if (m_CheckBox[1].GetCheck())
		pUnit->iItem |= SAPPHIRE;
	if (m_CheckBox[2].GetCheck())
		pUnit->iItem |= DIAMOND;

	m_mapUnit.insert(make_pair(pUnit->wstrName, pUnit));

	// ����Ʈ�ڽ� ��Ͽ� ������ �߰�.
	m_ListBox.AddString(m_strName);
}


void CUnitTool::OnLbnSelectUnit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// GetCurSel: ���õ� ����� �ε����� ��ȯ.
	int iIndex = m_ListBox.GetCurSel();

	// �߸��� ����� �����Ѵٸ� -1�� ����.
	if (-1 == iIndex)
	{
		::AfxMessageBox(L"�߸��� ����Դϴ�.");
		return;
	}

	CString strFindName = L"";

	// �ε����� �ش��ϴ� �̸��� ����.
	m_ListBox.GetText(iIndex, strFindName);

	auto iter_find = m_mapUnit.find(strFindName.GetString());

	if (m_mapUnit.end() == iter_find)
		return;

	m_strName = iter_find->second->wstrName.c_str();
	m_iAtt = iter_find->second->iAtt;
	m_iDef = iter_find->second->iDef;

	for (int i = 0; i < 3; ++i)
	{
		m_RadioBtn[i].SetCheck(FALSE); // ��� ��ư üũ ����
		m_CheckBox[i].SetCheck(FALSE); // ��� ��ư üũ ����
	}

	int iJobIndex = iter_find->second->iJobIndex;
	m_RadioBtn[iJobIndex].SetCheck(TRUE);

	int iItem = iter_find->second->iItem;

	if (iItem & RUBY)
		m_CheckBox[0].SetCheck(TRUE);
	if (iItem & SAPPHIRE)
		m_CheckBox[1].SetCheck(TRUE);
	if (iItem & DIAMOND)
		m_CheckBox[2].SetCheck(TRUE);

	UpdateData(FALSE);
}

void CUnitTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���� ���� Ȥ�� ���� ���� ���� ��ȭ���ڸ� �������ִ� MFC Ŭ����.
	CFileDialog Dlg(FALSE, L".dat", L"���� ����.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files|*.dat|Text Files|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	// ���� �۾���θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentDir);

	// ���� ��ο��� ���ϸ��� �����ϴ� �Լ�. ���ϸ��� ���ٸ� ���� �������� ����.
	::PathRemoveFileSpec(szCurrentDir);
	//::AfxMessageBox(szCurrentDir);

	// PathCombine(A, B, C)
	// B�� C�� ������ ��θ� A�� ����. �� �� B�� C ���̿� \\ ����.
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	// ��ȭ���� �ʱ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	// â�� �����ϴ� 2���� ���
	// Modal: ������� ���� â���� ����. DoMadal�Լ�
	// Modeless: ���� â �Ӹ��ƴ϶� �ٸ� â���Ե� ������� ����. Create�Լ�
	if (IDOK == Dlg.DoModal())
	{
		// ��ȭ���ڿ� �Է��� �����̸�(��α��� ���Ե�)�� ������ �Լ�. 
		//Dlg.GetPathName()
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, nullptr,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwBytes = 0;

		for (auto& MyPair : m_mapUnit)
		{
			for (size_t i = 0; i < MyPair.second->wstrName.length() + 1; ++i)
				WriteFile(hFile, &MyPair.second->wstrName[i], sizeof(wchar_t), &dwBytes, nullptr);

			WriteFile(hFile, &MyPair.second->iAtt, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &MyPair.second->iDef, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &MyPair.second->iJobIndex, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &MyPair.second->iItem, sizeof(int), &dwBytes, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L".dat", L"���� ����.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files|*.dat|Text Files|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, nullptr,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		if (!m_mapUnit.empty())
		{
			for (auto& MyPair : m_mapUnit)
				SafeDelete(MyPair.second);

			m_mapUnit.clear();
			m_ListBox.ResetContent(); // ����Ʈ�ڽ� ��� ����.
		}

		DWORD dwBytes = 0;
		wstring wstrName = L"";
		wchar_t ch = 0;
		int iAtt = 0, iDef = 0, iJobIndex = 0, iItem = 0;
		UNIT_INFO* pUnit = nullptr;

		while(true)
		{
			wstrName = L"";

			while (true)
			{
				ReadFile(hFile, &ch, sizeof(wchar_t), &dwBytes, nullptr);						

				if (0 == ch)
					break;

				wstrName += ch;
			}

			ReadFile(hFile, &iAtt, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iDef, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iJobIndex, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iItem, sizeof(int), &dwBytes, nullptr);

			if (0 == dwBytes)
				break;

			pUnit = new UNIT_INFO;
			pUnit->wstrName = wstrName;
			pUnit->iAtt = iAtt;
			pUnit->iDef = iDef;
			pUnit->iJobIndex = iJobIndex;
			pUnit->iItem = iItem;

			m_mapUnit.insert(make_pair(pUnit->wstrName, pUnit));
			m_ListBox.AddString(pUnit->wstrName.c_str());
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnEnChangeFindName()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	auto iter_find = m_mapUnit.find(m_strFindName.GetString());

	if (m_mapUnit.end() == iter_find)
		return;

	// ã�Ƴ� �ε����� ��ȯ.
	int iIndex = m_ListBox.FindString(-1, m_strFindName);

	if (-1 == iIndex)
		return;

	// �ε����� �����ϰ� ����.
	m_ListBox.SetCurSel(iIndex);
	CUnitTool::OnLbnSelectUnit();
}


void CUnitTool::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strFindName = L"";
	m_ListBox.GetText(iIndex, strFindName);

	auto iter_find = m_mapUnit.find(strFindName.GetString());

	if (m_mapUnit.end() == iter_find)
		return;

	SafeDelete(iter_find->second);
	m_mapUnit.erase(iter_find);

	// �ش� �ε����� ������ ������.
	m_ListBox.DeleteString(iIndex);	

	// ���� �� �ʱ�ȭ.
	m_strName = L"";
	m_iAtt = 0;
	m_iDef = 0;

	for (int i = 0; i < 3; ++i)
	{
		m_RadioBtn[i].SetCheck(FALSE); // ��� ��ư üũ ����
		m_CheckBox[i].SetCheck(FALSE); // ��� ��ư üũ ����
	}

	UpdateData(FALSE);
}
