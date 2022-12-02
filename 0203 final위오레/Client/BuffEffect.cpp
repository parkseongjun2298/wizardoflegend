#include "stdafx.h"
#include "BuffEffect.h"
#include "EffectIMP.h"
#include "Player.h"
#include "ScrollMgr.h"

CBuffEffect::CBuffEffect()
	: m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0.f), m_fCount(0.f),
	m_fLifeTime(0.f), m_fTimeCount(0.f), m_bCheck(false)
{
}


CBuffEffect::~CBuffEffect()
{
	Release();
}

int CBuffEffect::Update()
{

	m_fTimeCount += m_pTimeMgr->GetDeltaTime();

	if (m_fLifeTime <= m_fTimeCount)
		return DEAD_OBJ;



	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	
	NULL_CHECK_RETURN(pPlayer, DEAD_OBJ);

	D3DXMATRIX matScale, matRotZ, matTrans, matRevZ, matWorld;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle)); // 공전의 반대로 자전시키면 자전이 상쇄된다. 여기서 +m_fAngle하면 가이아방패
	D3DXMatrixTranslation(&matTrans,
		m_fDist,
		0.f, 0.f);
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(-m_fAngle));

	m_tInfo.matWorld = matScale *matRotZ * matTrans * matRevZ * pPlayer->GetInfo().matWorld;

	D3DXMatrixTranslation(&matTrans, CScrollMgr::GetScrollMove().x, CScrollMgr::GetScrollMove().y, 0.f);
	matWorld = m_tInfo.matWorld * matTrans;
	



	D3DXVec3TransformCoord(&m_tInfo.vPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);
	

	// 초당 90도 회전
	m_fAngle += m_fCount * m_pTimeMgr->GetDeltaTime();
	
	m_fCount-=2;//게이지 풀일때 15
	if (m_fDist >= 140)
	{
		m_bCheck = true;
	}
	if(m_fDist < 140 && m_bCheck==false)
	{ 
		m_fDist +=0.4;
		
	}
	if (m_fDist < 140 && m_bCheck == true)
	{
		m_fDist -= 1.1;
		
	}
	if (m_fDist >= 140 && m_bCheck == true)
	{
		m_fDist -=1;
		
	}
	
	return m_pImp->Update();


}

void CBuffEffect::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CBuffEffect::Render()
{
	m_pImp->Render(m_tInfo.matWorld);
}

HRESULT CBuffEffect::Initialize()
{
	//m_tInfo.vPos = { 105.f, 0.f, 0.f };
	m_fDist = 105.f;
	m_tInfo.vSize = { 1.2f, 1.2f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fCount = -200;
	m_tInfo.vPictureSize = { 106.f, 113.f, 0.f };
	return S_OK;
}

void CBuffEffect::Release()
{
}

CBuffEffect* CBuffEffect::Create(CEffectIMP* pImp, float fAngle,float fTime)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CBuffEffect* pInstance = new CBuffEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_fAngle = fAngle;
	pInstance->m_pImp = pImp;
	pInstance->m_fLifeTime = fTime;
	return pInstance;
}
