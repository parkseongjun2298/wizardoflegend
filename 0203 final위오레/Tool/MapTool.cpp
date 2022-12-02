// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"
#include "afxdialogex.h"

// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_byDrawID(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelectTile)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


// WM_DROPFILES �޽��� �߻��� �� ȣ���.
void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	//::AfxMessageBox(L"���� ������ ���Ҵ�?");

	TCHAR szFileName[MAX_STR] = L"";
	TCHAR szRelative[MAX_STR] = L"";

	// �ι�° ���ڿ� -1�� �����ϸ� �巡�� �� ��ӵ� ��ü ����� ������ ��ȯ�Ѵ�.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (int i = 0; i < iCount; ++i)
	{
		// DragQueryFile(HDROP, index, filename, length)
		// �巡�� �� ��ӵ� ��� �� index��°�� �����̸�(�������)�� filename�� ����ϴ� �Լ�.
		::DragQueryFile(hDropInfo, i, szFileName, MAX_STR);

		// ������ -> �����
		CDirectoryMgr::ConvertRelativePath(szRelative, szFileName);

		// ���ϸ� ���ܺ���.
		TCHAR* pFileName = ::PathFindFileName(szRelative);
		lstrcpy(szFileName, pFileName);

		// Ȯ���� �����ϱ�.
		::PathRemoveExtension(szFileName);

		m_ListBox.AddString(szFileName);
	}	

	SetHorizontalScrollSizes();
}

void CMapTool::SetHorizontalScrollSizes()
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


void CMapTool::OnLbnSelectTile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strFileName = L"";
	m_ListBox.GetText(iIndex, strFileName);

	int i = 0;

	for ( ; i < strFileName.GetLength(); ++i)
	{
		// ���� ���ڰ� ���ڶ�� true��ȯ. ���ڶ�� false��ȯ.
		if (isdigit(strFileName[i]))
			break;
	}

	// CString::Delete(index, count): ���� ���ڿ��� index���� count��ŭ ����.
	strFileName.Delete(0, i);

	// _ttoi: ���� -> ������ ��ȯ.
	m_byDrawID = _ttoi(strFileName);

	// ���õ� Ÿ�� �̸����� ���
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_byDrawID);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (float)WINCX / TILECX, (float)WINCY / TILECY, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	m_pDeviceMgr->Render_Begin();

	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDeviceMgr->Render_End(m_PictureCtrl.m_hWnd);
}


void CMapTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, L".dat", L"���� ����.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files|*.dat|Text Files|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		pToolView->m_pTerrain->SaveData(Dlg.GetPathName());
	}
}


void CMapTool::OnBnClickedLoad()
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
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		pToolView->m_pTerrain->LoadData(Dlg.GetPathName());
		pToolView->Invalidate(FALSE);
	}
}
