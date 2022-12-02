#include "stdafx.h"
#include "NormalEffect.h"
#include "EffectIMP.h"
#include "ScrollMgr.h"
#include"Player.h"

CNormalEffect::CNormalEffect()
	: m_pObjectMgr(CObjectMgr::GetInstance())
	,m_fAngle(0), m_fCount(0)
{
}


CNormalEffect::~CNormalEffect()
{
	Release();
}

int CNormalEffect::Update()
{
	m_fAngle += 0.2;

	D3DXMATRIX matScale, matTrans,matRotateZ, matRevZ;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollMove().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollMove().y, 0.f);
	D3DXMatrixRotationZ(&matRotateZ,
		D3DXToRadian(-m_fAngle));
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(-m_fAngle));

	m_tInfo.matWorld = matScale /** matRotateZ*/*matTrans*matRevZ*pPlayer->GetInfo().matWorld;

	m_fAngle += m_fCount * m_pTimeMgr->GetDeltaTime();
	
	return m_pImp->Update();
}

void CNormalEffect::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CNormalEffect::Render()
{
	m_pImp->Render(m_tInfo.matWorld);
}

HRESULT CNormalEffect::Initialize()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fCount = 0;
	return S_OK;
}

void CNormalEffect::Release()
{
}

CNormalEffect* CNormalEffect::Create(const D3DXVECTOR3& vPos, CEffectIMP* pImp)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CNormalEffect* pInstance = new CNormalEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_pImp = pImp;
	pInstance->m_tInfo.vPos = vPos;

	return pInstance;
}
