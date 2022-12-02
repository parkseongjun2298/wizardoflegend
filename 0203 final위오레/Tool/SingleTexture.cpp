#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEX_INFO* CSingleTexture::GetTexInfo(
	const wstring& wstrStateKey, 
	int iIndex) const
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::LoadTexture(
	LPDIRECT3DDEVICE9 pGraphicDev,
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey/* = L""*/, 
	int iCount/* = 0*/)
{
	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(tImgInfo));

	// �̹��� ������ ������ �Լ�.
	HRESULT hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &tImgInfo);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

	LPDIRECT3DTEXTURE9	pTexture = nullptr;

	// �̹��� �ε� �Լ�. (IDirect3DTexture9 COM ��ü ����)
	hr = D3DXCreateTextureFromFileEx(
		pGraphicDev, 
		wstrFilePath.c_str(), 
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

	m_pTexInfo = new TEX_INFO;
	m_pTexInfo->pTexture = pTexture;
	m_pTexInfo->tImgInfo = tImgInfo;

	return S_OK;
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	SafeDelete(m_pTexInfo);
}

CSingleTexture* CSingleTexture::Create(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	const wstring& wstrFilePath)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CSingleTexture* pInstance = new CSingleTexture;

	if (FAILED(pInstance->LoadTexture(pGraphicDev, wstrFilePath)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
