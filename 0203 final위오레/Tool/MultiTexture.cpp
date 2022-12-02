#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEX_INFO* CMultiTexture::GetTexInfo(
	const wstring& wstrStateKey/* = L""*/, 
	int iIndex/* = 0*/) const
{
	auto iter_find = m_MultiTexture.find(wstrStateKey);

	if(m_MultiTexture.end() == iter_find)
		return nullptr;

	if (0 > iIndex || iter_find->second.size() <= (size_t)iIndex)
		return nullptr;

	return iter_find->second[iIndex];
}

HRESULT CMultiTexture::LoadTexture(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey/* = L""*/, 
	int iCount/* = 0*/)
{
	auto iter_find = m_MultiTexture.find(wstrStateKey);

	if (m_MultiTexture.end() != iter_find)
		return E_FAIL;

	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(tImgInfo));

	LPDIRECT3DTEXTURE9	pTexture = nullptr;
	TCHAR szFullPath[MAX_STR] = L"";

	TEX_INFO* pTexInfo = nullptr;

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, wstrFilePath.c_str(), i);

		// �̹��� ������ ������ �Լ�.
		HRESULT hr = D3DXGetImageInfoFromFile(szFullPath, &tImgInfo);
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

		// �̹��� �ε� �Լ�. (IDirect3DTexture9 COM ��ü ����)
		hr = D3DXCreateTextureFromFileEx(
			pGraphicDev,
			szFullPath,
			tImgInfo.Width,		/* ������ �ؽ�ó�� ���� ũ�� */
			tImgInfo.Height,	/* ������ �ؽ�ó�� ���� ũ�� */
			tImgInfo.MipLevels, /* �Ӹ� �ܰ� */
			0, /* Usage: �ؽ�ó ����� */
			tImgInfo.Format, /* �ؽ�ó�� �ȼ� ���� */
			D3DPOOL_MANAGED, /* �޸� ���� ��� */
			D3DX_DEFAULT,	/* ���͸� ��� */
			D3DX_DEFAULT,	/* ���͸� ��� */
			0,	/* ������ ���� Ű */
			nullptr, /* D3DXIMAGE_INFO�� ������ */
			nullptr, /* PALETTEENTRY�� ������ */
			&pTexture);
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateTextureFromFileEx Failed", E_FAIL);	

		pTexInfo = new TEX_INFO;
		pTexInfo->pTexture = pTexture;
		pTexInfo->tImgInfo = tImgInfo;

		m_MultiTexture[wstrStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for (auto& MyPair : m_MultiTexture)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), 
			[](auto& pTexInfo)
		{
			pTexInfo->pTexture->Release();
			SafeDelete(pTexInfo);
		});

		MyPair.second.clear();
	}

	m_MultiTexture.clear();
}

CMultiTexture* CMultiTexture::Create(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey, 
	int iCount)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CMultiTexture* pInstance = new CMultiTexture;

	if (FAILED(pInstance->LoadTexture(pGraphicDev, wstrFilePath, wstrStateKey, iCount)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
