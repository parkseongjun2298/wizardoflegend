#include "stdafx.h"
#include "HitEffect.h"
#include"ScrollMgr.h"
#include"TimeMgr.h"

CHitEffect::CHitEffect()
	:m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fAngle(0.f)
{
}


CHitEffect::~CHitEffect()
{
	Release();
}

int CHitEffect::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime()*8;

	if (m_fFrameStart >= m_fMaxFrameCount)
	{
		m_fFrameStart = 0.f;
		return DEAD_OBJ;
	}
	

	return NO_EVENT;
}

void CHitEffect::LateUpdate()
{
	m_bCheck = true;
	D3DXMATRIX matScale, matRotZ, matTrans, matPar;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*0.5,
		m_tInfo.vSize.y*0.5, 1.f);
	D3DXMatrixTranslation(&matTrans,   //나오는거리
		m_tInfo.vDir.x * -40.f,
		m_tInfo.vDir.y * -40.f, 0.f);

	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matPar,
		m_tInfo.vPos.x - CScrollMgr::GetScrollMove().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollMove().y, 0.f);



	m_tInfo.matWorld = matScale * matRotZ * matTrans * matPar;



}

void CHitEffect::Render()
{
	if (!m_bCheck)
		return;
	
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Effect", L"HiteEffect", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CHitEffect::Initialize()
{
	
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 8.f;

	bCoolCheck = true;

	m_bCheck = false;
	//	m_fAngle = 5;
	return S_OK;
}

void CHitEffect::Release()
{
}

CHitEffect * CHitEffect::Create(D3DXVECTOR3 _Pos)
{
	CHitEffect* pInstance = new CHitEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _Pos.x;
	pInstance->GetInfo().vPos.y = _Pos.y;
	return pInstance;
}
