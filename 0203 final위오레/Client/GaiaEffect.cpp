#include "stdafx.h"
#include "GaiaEffect.h"
#include "EffectIMP.h"
#include "Player.h"
#include"ScrollMgr.h"
CGaiaEffect::CGaiaEffect()
	: m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0.f), m_fCount(0.f),
	m_fLifeTime(0.f), m_fTimeCount(0.f), m_bCheck(false)
{
}


CGaiaEffect::~CGaiaEffect()
{
	Release();
}

int CGaiaEffect::Update()
{

	if (m_fLifeTime <= m_fTimeCount)
		return DEAD_OBJ;



	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());

	NULL_CHECK_RETURN(pPlayer, DEAD_OBJ);

	D3DXMATRIX matScale, matRotZ, matTrans, matRevZ, matWorld;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle)); // ������ �ݴ�� ������Ű�� ������ ���ȴ�. ���⼭ +m_fAngle�ϸ� ���̾ƹ���
	D3DXMatrixTranslation(&matTrans,
		m_fDist,
		0.f, 0.f);
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(-m_fAngle));

	m_tInfo.matWorld = matScale *matRotZ * matTrans * matRevZ * pPlayer->GetInfo().matWorld;
	D3DXMatrixTranslation(&matTrans, CScrollMgr::GetScrollMove().x, CScrollMgr::GetScrollMove().y, 0.f);
	matWorld = m_tInfo.matWorld * matTrans;

	D3DXVec3TransformCoord(&m_tInfo.vPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);

	// �ʴ� 90�� ȸ��
	m_fAngle += m_fCount * m_pTimeMgr->GetDeltaTime();

	

	return m_pImp->Update();
}

void CGaiaEffect::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CGaiaEffect::Render()
{
	m_pImp->Render(m_tInfo.matWorld);
}

HRESULT CGaiaEffect::Initialize()
{
	//m_tInfo.vPos = { 105.f, 0.f, 0.f };
	m_fDist = 105.f;
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fCount = -100;
	m_tInfo.vPictureSize = { 71.f,100.f,0.f };
	return S_OK;
}

void CGaiaEffect::Release()
{
}

CGaiaEffect * CGaiaEffect::Create(CEffectIMP * pImp, float fAngle,float Time)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CGaiaEffect* pInstance = new CGaiaEffect;

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
