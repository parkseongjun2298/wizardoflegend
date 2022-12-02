#include "stdafx.h"
#include "DirectoryMgr.h"


CDirectoryMgr::CDirectoryMgr()
{
}


CDirectoryMgr::~CDirectoryMgr()
{
}

void CDirectoryMgr::ConvertRelativePath(TCHAR* pOut, const TCHAR* pTo)
{
	// pFrom���� pTo�� ã�ư��� ����θ� ���� pOut�� �������ִ� �Լ�.
	// ��, pFrom�� pTo�� ���� ����̺꿡 �����ؾ���.
	//::PathRelativePathTo(pOut, pFrom, FILE_ATTRIBUTE_DIRECTORY,
	//	pTo, FILE_ATTRIBUTE_DIRECTORY);

	TCHAR szCurrentDir[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentDir);

	::PathRelativePathTo(pOut, szCurrentDir, FILE_ATTRIBUTE_DIRECTORY,
		pTo, FILE_ATTRIBUTE_DIRECTORY);
}

void CDirectoryMgr::ExtractPathInfo(CString strFilePath, list<PATH_INFO*>& rPathInfoList)
{
	strFilePath += L"\\*.*";

	// ��� Ž�� ���� MFC Ŭ����.
	CFileFind	find;

	// ��� ������ �����ϴ� �Լ�. �ش� ��ΰ� �����ϸ� TRUE ��ȯ.
	// ��ΰ� �����߰� �ش� ��ο� �����ϱ� ���ؼ��� �ݵ�� FindNextFile�� ȣ�����־���Ѵ�.
	// ����, �����ϴ� ��ο� "*.*"�� ���Ե� ��쿡 FindNextFile�� ȣ���ϸ� ���� ù��° ��󿡰� ������ �� �ִ�.
	BOOL bIsFind = find.FindFile(strFilePath);

	while (bIsFind)
	{
		// ���� ������ ���Ͽ� ������ ��� FALSE�� ��ȯ.
		bIsFind = find.FindNextFile();

		if(find.IsDots()) // ã�Ƴ� ����� .�� ..���� ��Ŀ�� ��� �ǳʶ�.
			continue;
		else if (find.IsDirectory()) // ã�Ƴ� ����� ������ ��� TRUE
			ExtractPathInfo(find.GetFilePath(), rPathInfoList);
		else // ã�Ƴ� ����� ������ ��
		{
			if(find.IsSystem()) // ã�Ƴ� ����� �ý��� ���� �����̶�� �ǳʶ�.
				continue;

			// �����
			CString str = find.GetFilePath();
			str.Replace(L"0.", L"%d."); // Replace(Old, New)

			TCHAR szRelative[MAX_STR] = L"";
			ConvertRelativePath(szRelative, str);

			PATH_INFO* pPathInfo = new PATH_INFO;
			pPathInfo->wstrRelativePath = szRelative;

			// �̹��� ���� ����
			::PathRemoveFileSpec(szRelative);
			pPathInfo->iFileCount = CountFile(szRelative);

			// StateKey
			pPathInfo->wstrStateKey = ::PathFindFileName(szRelative);

			// ObjectKey
			::PathRemoveFileSpec(szRelative);
			pPathInfo->wstrObjectKey = ::PathFindFileName(szRelative);

			rPathInfoList.push_back(pPathInfo);
			break;
		}
	}
}

int CDirectoryMgr::CountFile(CString strFilePath)
{
	strFilePath += L"\\*.*";

	CFileFind find;
	BOOL bIsFind = find.FindFile(strFilePath);

	int iCount = 0;

	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if (find.IsDots() || find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;
}
