#include "stdafx.h"
#include "ArcherSummon.h"
#include "EffectIMP.h"
#include"Archer.h"

CArcherSummon::CArcherSummon()
	: m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0.f), m_fCount(0.f),
	m_fLifeTime(0.f), m_fTimeCount(0.f), m_bCheck(false)
{
}


CArcherSummon::~CArcherSummon()
{
	Release();
}

int CArcherSummon::Update()
{
	if (m_fLifeTime <= m_fTimeCount)
		return DEAD_OBJ;
		
	//CArcher* pArcher=dynamic_cast<CArcher*>(m_pObjectMgr->GetMonster());
	//NULL_CHECK_RETURN(pArcher, DEAD_OBJ);


	D3DXMATRIX matScale, matRotZ, matTrans, matRevZ;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle)); // 공전의 반대로 자전시키면 자전이 상쇄된다. 여기서 +m_fAngle하면 가이아방패
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(-m_fAngle));

	m_tInfo.matWorld = matScale  * matTrans  * m_pOwner->GetInfo().matWorld;

	// 초당 90도 회전
	m_fAngle += m_fCount * m_pTimeMgr->GetDeltaTime();



	return m_pImp->Update();
}

void CArcherSummon::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CArcherSummon::Render()
{
	m_pImp->Render(m_tInfo.matWorld);
}

HRESULT CArcherSummon::Initialize()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.7f, 2.7f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fCount = -100;

	return S_OK;
}

void CArcherSummon::Release()
{
}

CArcherSummon * CArcherSummon::Create(CGameObject* pOwner, CEffectIMP * pImp, float fAngle, float Time)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CArcherSummon* pInstance = new CArcherSummon;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_fAngle = fAngle;
	pInstance->m_pImp = pImp;
	pInstance->m_fLifeTime = Time;
	pInstance->m_pOwner = pOwner;
	return pInstance;
}
