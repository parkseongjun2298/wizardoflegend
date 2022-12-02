#include "stdafx.h"
#include "BossShadow.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"

CBossShadow::CBossShadow()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fSpeed(0), m_fFrameStart(0), m_fMaxFrameCount(0), m_fAngle(0)
{
}


CBossShadow::~CBossShadow()
{
	Release();
}

int CBossShadow::Update()
{
	m_fCoolCount += m_pTimeMgr->GetDeltaTime();

	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
		m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	if (pPlayer->GetInfo().vPos.x < m_tInfo.vPos.x)
		m_tInfo.vSize = { -1.f, 1.f, 0.f };
	else
		m_tInfo.vSize = { 1.f, 1.f, 0.f };
	if (m_bCheck)
	{
		

		if (m_fCoolCount >= 13.f&&m_fCoolCount < 15.5)
		{
			m_eCurState = ATTACK;
			
		}
		if(m_eCurState ==ATTACK)
		{ 
		D3DXVec3Normalize(&m_Dir, &m_Dir);
		D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);

		m_tInfo.vPos += m_Dir*m_fSpeed;

		}

		if (m_fCoolCount >= 15.5f&&m_fCoolCount < 18)
		{
			m_eCurState = IDLE;

		}
	}
	
	
	if (m_fCoolCount >= 23.f &&m_fCoolCount < 24.f)
	{

		m_fCoolCount = 0;

	}



	return NO_EVENT;
}

void CBossShadow::LateUpdate()
{
	D3DXMATRIX matScale, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*0.6,
		m_tInfo.vSize.y*0.6, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollMove().x,
		m_tInfo.vPos.y+90 - CScrollMgr::GetScrollMove().y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale *matRotZ* matTrans;

	m_tInfo.matWorld = matScale * matTrans;


	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vInitialDir, &matWorld);
}

void CBossShadow::Render()
{
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Boss_Shadow", m_wstStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBossShadow::Initialize()
{
	
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fSpeed = 5.f;

	m_eCurState = IDLE;
	m_wstStateKey = L"Idle";

	m_bCheck = true;
	m_bHit = true;
	return S_OK;
}

void CBossShadow::Release()
{
}

CBossShadow * CBossShadow::Create(D3DXVECTOR3 _PPos)
{
	CBossShadow* pInstance = new CBossShadow;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	
	pInstance->GetInfo().vPos = _PPos;
	return pInstance;
}

void CBossShadow::StateKey()
{
	switch (m_eCurState)
	{
	case CBossShadow::IDLE:
		m_wstStateKey = L"Idle";
		m_fMaxFrameCount = 1;
		break;
	case CBossShadow::ATTACK:
		m_wstStateKey = L"Attack";
		m_fMaxFrameCount = 1;
		break;
	default:
		break;
	}
}
