#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO* CTextureMgr::GetTexInfo(
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	int iIndex) const
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if(m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, iIndex);
}

HRESULT CTextureMgr::LoadTexture(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	TEXTURE_TYPE eType, 
	const wstring& wstrFilePath, 
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	int iCount)
{
	NULL_CHECK_RETURN(pGraphicDev, E_FAIL);

	auto iter_find = m_mapTexture.find(wstrObjectKey);
	bool bIsExist = (m_mapTexture.end() != iter_find);

	CTexture* pTexture = nullptr;

	switch (eType)
	{
	case SINGLE_TEXTURE:
		if (!bIsExist)
		{
			pTexture = CSingleTexture::Create(pGraphicDev, wstrFilePath);
			NULL_CHECK_RETURN(pTexture, E_FAIL);
			m_mapTexture.insert(make_pair(wstrObjectKey, pTexture));
		}
		else
			return E_FAIL;
		break;
	case MULTI_TEXTURE:
		if (!bIsExist)
		{
			pTexture = CMultiTexture::Create(pGraphicDev, wstrFilePath, wstrStateKey, iCount);
			NULL_CHECK_RETURN(pTexture, E_FAIL);
			m_mapTexture.insert(make_pair(wstrObjectKey, pTexture));
		}
		else
		{
			HRESULT hr = m_mapTexture[wstrObjectKey]->LoadTexture(
				pGraphicDev, wstrFilePath, wstrStateKey, iCount);
			FAILED_CHECK_RETURN(hr, E_FAIL);
		}
		break;
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& MyPair : m_mapTexture)
		SafeDelete(MyPair.second);

	m_mapTexture.clear();
}
