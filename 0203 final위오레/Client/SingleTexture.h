#pragma once
#include "Texture.h"

class CSingleTexture : public CTexture
{
private:
	CSingleTexture();

public:
	virtual ~CSingleTexture();

public:
	// CTexture��(��) ���� ��ӵ�
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"",
		int iIndex = 0) const override;

public:
	// CTexture��(��) ���� ��ӵ�
	virtual HRESULT LoadTexture(
		LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		int iCount = 0) override;

private:
	virtual void Release() override;

public:
	static CSingleTexture* Create(
		LPDIRECT3DDEVICE9 pGraphicDev, 
		const wstring& wstrFilePath);

private:
	TEX_INFO*	m_pTexInfo;
};

