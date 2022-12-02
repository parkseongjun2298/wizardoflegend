#include "stdafx.h"
#include "SummonAni.h"


CSummonAni::CSummonAni()
	: m_fFrameStart(0.f),
	m_fMaxFrameCount(0.f),
	m_fLifeTime(0.f), m_fTimeCount(0.f)
{
}


CSummonAni::~CSummonAni()
{
	Release();
}

int CSummonAni::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0;
	//return DEAD_OBJ;
	m_fTimeCount += m_pTimeMgr->GetDeltaTime();

	if (m_fLifeTime <= m_fTimeCount)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CSummonAni::LateUpdate()
{
}

void CSummonAni::Render(const D3DXMATRIX & matWorld)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CSummonAni::Initialize()
{
	m_fLifeTime = 1;

	return S_OK;
}

void CSummonAni::Release()
{
}

CSummonAni * CSummonAni::Create(const wstring & wstrObjectKey, const wstring & wstrStateKey, const float & fFrameStart, const float & fMaxFrameCount)
{
	CSummonAni* pInstance = new CSummonAni;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_wstrStateKey = wstrStateKey;
	pInstance->m_fFrameStart = fFrameStart;
	pInstance->m_fMaxFrameCount = fMaxFrameCount;

	return pInstance;
}
