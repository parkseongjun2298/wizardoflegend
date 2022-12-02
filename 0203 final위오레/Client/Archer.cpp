#include "stdafx.h"
#include "Archer.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"
#include"ArcherBow.h"
#include"HitEffect.h"
CArcher::CArcher()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f),  
	m_fAngle(0), m_fSpeed(0), m_fCoolTime(0), m_fHitTime(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CArcher::~CArcher()
{
	Release();
}

int CArcher::Update()
{


	if (m_bDie)

	{
		m_eCurState = DIE;
		CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney += 5;
		m_fDieCount += m_pTimeMgr->GetDeltaTime();
		if (m_fDieCount >= 0.5)
		{
			CSoundMgr::GetInstance()->PlaySound(L"ENEMY_DIED_2.mp3", CSoundMgr::UI);
			return DEAD_OBJ;
		}
	}

	StateKey();

	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_fMaxFrameCount)
	{
		m_fFrameStart = 0.f;
		m_bBowCheck = true;
	}

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	if (pPlayer->GetInfo().vPos.x < m_tInfo.vPos.x)
		m_tInfo.vSize = { -1.f, 1.f, 0.f };
	else
		m_tInfo.vSize = { 1.f, 1.f, 0.f };

	D3DXVECTOR3 Temp = pPlayer->GetInfo().vPos - m_tInfo.vPos;
	if (m_bCheck&&D3DXVec3Length(&Temp) > 350.f)
	{
		D3DXVec3Normalize(&m_Dir, &m_Dir);
		D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);
		m_tInfo.vPos += m_Dir*m_fSpeed;
		if (m_Dir != 0)
			m_eCurState = WALK;
	}

	
	if (D3DXVec3Length(&Temp)<250.f)
	{
		D3DXVec3Normalize(&m_Dir, &m_Dir);
		D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);
		m_tInfo.vPos -= m_Dir*m_fSpeed;
		if (m_Dir != 0)
			m_eCurState = WALK;
	}
	if (D3DXVec3Length(&Temp) <= 350.f&&D3DXVec3Length(&Temp) >= 250.f)
	{
		m_eCurState = ATTACK;
		//활소환
		if(m_bBowCheck && m_eCurState == ATTACK)
		{ 
			CSoundMgr::GetInstance()->PlaySound(L"ARCHER_AIM.mp3", CSoundMgr::UI);
		CArcherBow* ArcherBow = CArcherBow::Create(this, m_tInfo.vPos, m_tInfo.vDir, m_fAngle);
		m_pObjectMgr->AddObject(MONSTEREFFECT, ArcherBow);
		m_bBowCheck = false;
		}
		m_bCheck = false;
	}

	if (!m_bCheck)
		m_fCoolTime += m_pTimeMgr->GetDeltaTime();

	if (m_fCoolTime >= 1.f)
	{
		m_bCheck = true;
		m_fCoolTime -= 1.f;
	}
	if (m_bHit == false)
	{
		m_eCurState = HIT;
		CHitEffect* pEffect = CHitEffect::Create(m_tInfo.vPos);
		m_pObjectMgr->AddObject(HITEFFECT, pEffect);
		m_fHitTime += m_pTimeMgr->GetDeltaTime();
		if (m_fHitTime >= 0.2)
		{
			CSoundMgr::GetInstance()->PlaySound(L"ENEMY_HITED_1.mp3", CSoundMgr::UI);
			m_fHitTime = 0;
			m_bHit = true;
		}

	}

	return NO_EVENT;
}

void CArcher::LateUpdate()
{
	D3DXMATRIX matScale, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollMove().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollMove().y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale *matRotZ* matTrans;

	m_tInfo.matWorld = matScale * matTrans;


	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vInitialDir, &matWorld);
}

void CArcher::Render()
{
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"ArcherRight", m_wstStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

HRESULT CArcher::Initialize()
{
	m_tInfo.vPos = { 2500, 48 * 40, 0.f };//2500,1920
										
	m_tInfo.vPictureSize = { 142,145,0 };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fSpeed = 3.f;

	m_eCurState = IDLE;
	m_wstStateKey = L"ArcherIdle";

	m_bHit = true;
	m_bBowCheck = true;
	m_bCheck = false;
	return S_OK;
}

void CArcher::Release()
{
}

CArcher * CArcher::Create(float _X,float _Y)
{
	CArcher* pInstance = new CArcher;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _X;
	pInstance->GetInfo().vPos.y = _Y;
	return pInstance;
}

void CArcher::StateKey()
{
	switch (m_eCurState)
	{
	case CArcher::IDLE:
		m_wstStateKey = L"ArcherIdle";
		m_fMaxFrameCount = 1;
		break;
	case CArcher::ATTACK:
		m_wstStateKey = L"ArcherAttack";
		m_fMaxFrameCount = 4;
		break;
	
	case CArcher::HIT:
		m_wstStateKey = L"ArcherHit";
		m_fMaxFrameCount = 2;
		break;
	case CArcher::WALK:
		m_wstStateKey = L"ArcherWalk";
		m_fMaxFrameCount = 5;
		break;
	case CArcher::DIE:
		m_wstStateKey = L"ArcherDie";
		m_fMaxFrameCount = 6;
		break;
	default:
		break;
	}
}
