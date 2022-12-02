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
	const wstring& GetLoadingMessage() const;
public:
	void SetLoadingMessage(const wstring& wstrMessage);
public:
	HRESULT LoadFromPathInfoFile(
		LPDIRECT3DDEVICE9 pGraphicDev, 
		const TCHAR* pFilePath);
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

	wstring		m_wstrLoadingMessage;
};

