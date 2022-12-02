#include "stdafx.h"
#include "SwordMan.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"
#include"SwordManEffect.h"
#include"HitEffect.h"

CSwordMan::CSwordMan()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fFrameIdleStart(0.f), m_fMaxFrameIdleCount(0.f), m_fFrameAttackStart(0.f),m_fMaxFrameAttackCount(0),
	m_fAngle(0), m_fSpeed(0), m_fCoolTime(0), m_fCoolCount(0), m_fHitTime(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));

}


CSwordMan::~CSwordMan()
{
	Release();
}



int CSwordMan::Update()
{
	//m_eCurState = IDLE;


	if (m_bDie)
		
	{
		

		m_eCurState = DIE;
		m_fDieCount +=   m_pTimeMgr->GetDeltaTime();
		CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney += 3;
		if(m_fDieCount>=0.5)
		{ 
			CSoundMgr::GetInstance()->PlaySound(L"ENEMY_DIED_2.mp3", CSoundMgr::UI);
			return DEAD_OBJ;
		}
	}
	StateKey();

	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	if (pPlayer->GetInfo().vPos.x < m_tInfo.vPos.x)
		m_tInfo.vSize = { 1.f, 1.f, 0.f };
	else
		m_tInfo.vSize = { -1.f, 1.f, 0.f };

	if (m_bCheck)
	{
		D3DXVec3Normalize(&m_Dir, &m_Dir);
		D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);
		m_tInfo.vPos += m_Dir*m_fSpeed;
		if (m_Dir != 0)
			m_eCurState = WALK;
	}

	D3DXVECTOR3 Temp = pPlayer->GetInfo().vPos - m_tInfo.vPos;
	
	if(D3DXVec3Length(&Temp) <= 50.f )
	{

		CSoundMgr::GetInstance()->PlaySound(L"SWORDMAN_ATTACK.mp3", CSoundMgr::UI); 
		m_eCurState = ATTACK;
		m_bCheck = false;
		CSwordManEffect* pSEffect= CSwordManEffect::Create(m_tInfo.vPos, m_tInfo.vDir, m_fAngle);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pSEffect);
			

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
		if (m_fHitTime>=0.2)
		{
			CSoundMgr::GetInstance()->PlaySound(L"ENEMY_HITED_1.mp3", CSoundMgr::UI); 
			m_fHitTime =0;
			m_bHit = true;
		}

	}


	return NO_EVENT;
}

void CSwordMan::LateUpdate()
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

void CSwordMan::Render()
{
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"SwordManLeft",m_wstStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CSwordMan::Initialize()
{
	m_tInfo.vPictureSize = { 139,151,0 };
	//m_tInfo.vPos = { 3000, 48 * 40, 0.f };//2500,1920
	//CScrollMgr::InitializeScroll(m_tInfo.vPos - D3DXVECTOR3{ WINCX * 0.5f, WINCY * 0.5f, 0 });

	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fSpeed = 3.f;

	m_eCurState = IDLE;
	m_wstStateKey = L"Idle";

	m_bCheck = false;
	m_bHit = true;
	return S_OK;
}

void CSwordMan::Release()
{
}

CSwordMan * CSwordMan::Create(float _X,float _Y)
{
	CSwordMan* pInstance = new CSwordMan;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _X;
	pInstance->GetInfo().vPos.y = _Y;
	return pInstance;
}

void CSwordMan::StateKey()
{
	switch (m_eCurState)
	{
	case CSwordMan::IDLE:
		m_wstStateKey = L"Idle";
		m_fMaxFrameCount = 1;
		break;
	case CSwordMan::ATTACK:
		m_wstStateKey = L"Attack";
		m_fMaxFrameCount = 3;
		break;
	case CSwordMan::DASH:
		break;
	case CSwordMan::HIT:
		m_wstStateKey = L"Hit";
		m_fMaxFrameCount = 2;
		break;
	case CSwordMan::WALK:
		m_wstStateKey = L"Walk";
		m_fMaxFrameCount = 6;
		break;
	case CSwordMan::DIE:
		m_wstStateKey = L"Die";
		m_fMaxFrameCount = 6;
		break;
	default:
		break;
	}
}
