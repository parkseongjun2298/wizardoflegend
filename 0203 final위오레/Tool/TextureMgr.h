#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:	
	const TEX_INFO* GetTexInfo(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		int iIndex = 0) const;

public:
	HRESULT LoadTexture(
		LPDIRECT3DDEVICE9 pGraphicDev,
		TEXTURE_TYPE eType,
		const wstring& wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		int iCount = 0);

private:
	virtual void Release();

private:
	typedef map<wstring, CTexture*>	MAP_TEXTURE;
	MAP_TEXTURE m_mapTexture;
};

