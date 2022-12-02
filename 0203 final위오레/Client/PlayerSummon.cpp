#include "stdafx.h"
#include "PlayerSummon.h"
#include "EffectIMP.h"
#include"Player.h"

CPlayerSummon::CPlayerSummon()
	: m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0.f), m_fCount(0.f),
	m_fLifeTime(0.f), m_fTimeCount(0.f), m_bCheck(false)
{
}


CPlayerSummon::~CPlayerSummon()
{
	Release();
}

int CPlayerSummon::Update()
{
	if (m_fLifeTime <= m_fTimeCount)
		return DEAD_OBJ;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	NULL_CHECK_RETURN(pPlayer, DEAD_OBJ);

	D3DXMATRIX matScale, matRotZ, matTrans, matRevZ;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle)); // 공전의 반대로 자전시키면 자전이 상쇄된다. 여기서 +m_fAngle하면 가이아방패
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y-60, 0.f);
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(-m_fAngle));

	m_tInfo.matWorld = matScale  * matTrans * pPlayer->GetInfo().matWorld;

	// 초당 90도 회전
	m_fAngle += m_fCount * m_pTimeMgr->GetDeltaTime();



	return m_pImp->Update();
}

void CPlayerSummon::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CPlayerSummon::Render()
{
	m_pImp->Render(m_tInfo.matWorld);
}

HRESULT CPlayerSummon::Initialize()
{
	//m_tInfo.vPos = { 50 * 55, 48 * 40, 0.f };
	m_tInfo.vSize = { 0.8f, 0.8f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fCount = -100;

	return S_OK;
}

void CPlayerSummon::Release()
{
}

CPlayerSummon * CPlayerSummon::Create( CEffectIMP * pImp, float fAngle, float Time)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CPlayerSummon* pInstance = new CPlayerSummon;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_fAngle = fAngle;
	pInstance->m_pImp = pImp;
	pInstance->m_fLifeTime = Time;
	
	return pInstance;
}
