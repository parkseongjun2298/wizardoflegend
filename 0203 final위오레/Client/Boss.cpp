#include "stdafx.h"
#include "Boss.h"
#include "ScrollMgr.h"
#include"TimeMgr.h"
#include "NormalEffect.h"
#include "BuffEffect.h"
#include "AnimateIMP.h"
#include "NoAnimateIMP.h"
#include "Player.h"
#include"BoxAttack.h"
#include"BossSkill.h"
#include"BossSkill2.h"
#include"HitEffect.h"

#include"BossShadow.h"

CBoss::CBoss()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fCoolTime(0), m_fCoolCount(0), m_fAngle(0), m_fSpeed(0)
	, m_fCoolTime2(0), m_fCoolCount2(0)
	
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CBoss::~CBoss()
{
	Release();
}

int CBoss::Update()
{
	//cout << "X:" << m_tInfo.vPos.x << endl;

	//cout << "Y:" << m_tInfo.vPos.y << endl;

	StateKey();
	if(m_bSkill3)
	{ 
		CBossShadow* pMon = CBossShadow::Create(m_tInfo.vPos);
		m_pObjectMgr->AddObject(BOSSSHADOW, pMon);

		m_bSkill3 = false;



	}

	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;
	m_fCoolCount +=  m_pTimeMgr->GetDeltaTime();

	
	

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	if (pPlayer->GetInfo().vPos.x < m_tInfo.vPos.x)
		m_tInfo.vSize = { -1.f, 1.f, 0.f };
	else
		m_tInfo.vSize = { 1.f, 1.f, 0.f };

	if (m_bCheck)
	{
		D3DXVec3Normalize(&m_Dir, &m_Dir);
		D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);
	

		if (m_fCoolCount < 2.f)
		{
			CSoundMgr::GetInstance()->PlaySound(L"BOSS_POSE.mp3", CSoundMgr::BOSS);
			m_eCurState = HELLO;
		}
	}

	if(m_fCoolCount >= 2.f&&m_fCoolCount < 3.f)
		m_eCurState = READY;
	
		if(m_bSkill1&&m_fCoolCount>=3.f&&m_fCoolCount < 8.f)
		{
			CSoundMgr::GetInstance()->PlaySound(L"ROCK_THROW_START.mp3", CSoundMgr::BOSS);

			m_eCurState = ATTACK;
			CBoxAttack* pMon = CBoxAttack::Create(m_tInfo.vPos.x-100,m_tInfo.vPos.y,1, pPlayer->GetInfo().vPos);  //생성할때 멈출 시간 넘겨주기
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);
			

			CBoxAttack* pMon2 = CBoxAttack::Create(m_tInfo.vPos.x + 100, m_tInfo.vPos.y,2, pPlayer->GetInfo().vPos);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon2);


			CBoxAttack* pMon3 = CBoxAttack::Create(m_tInfo.vPos.x , m_tInfo.vPos.y+100,3, pPlayer->GetInfo().vPos);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon3);
			m_bSkill1 = false;

			
		}
		////1초 쿨줌
		
		//패턴 다시 하는 시간 조절부분



		if (m_fCoolCount >= 23.f &&m_fCoolCount < 24.f)
		{
			m_bSkill1 = true;
			m_bSkill2 = true;
			
			m_fCoolCount = 0;
		
		}

	
		m_bCheck = false;
	
	////////////////////////////////////////////////////////////////////////////


		if (m_fCoolCount >= 8.f&&m_fCoolCount < 9.f)

			m_eCurState = READY;
		if (m_fCoolCount >= 9.f&&m_fCoolCount<13)
		{ 
		m_eCurState = ATTACK;

		
			if (m_bSkill2)
			{
				m_fSkill2Cool += m_pTimeMgr->GetDeltaTime();

			m_eCurState = ATTACK;

			if (m_fSkill2Cool > 0.1f)
			{
				CSoundMgr::GetInstance()->PlaySound(L"COLUMN_2.mp3", CSoundMgr::BOSS);
				CBossSkill* pMon = CBossSkill::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.1);  //생성할때 멈출 시간 넘겨주기
				m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);
				m_fSkill2Cool -= 0.1f;
			}

			//m_bSkill2 = false;


			}
		}
		
		if (m_fCoolCount >= 13.f)
		{
			m_bSkill2 = false;
			
		}
		if (m_fCoolCount >= 13.f&&m_fCoolCount < 15.5)
		{
			m_eCurState = JUMPSKILL;

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
			m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

			m_tInfo.vPos -= m_Dir * 5;
			

		}
		if (m_fCoolCount >= 15.5f && m_fCoolCount < 18)
		{
			if (m_fCoolCount >= 15.5f && m_fCoolCount < 15.55)
			CSoundMgr::GetInstance()->PlaySound(L"BOSS_DROP.mp3", CSoundMgr::BOSS);

			m_tInfo.vPos = static_cast<CBossShadow*>(m_pObjectMgr->m_ObjectList[BOSSSHADOW].front())->GetInfo().vPos;
						
			CBossSkill2* pMon = CBossSkill2::Create(m_tInfo.vPos.x , m_tInfo.vPos.y+150, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			 pMon = CBossSkill2::Create(m_tInfo.vPos.x , m_tInfo.vPos.y-150, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x-150, m_tInfo.vPos.y, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x+150, m_tInfo.vPos.y, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x + 150, m_tInfo.vPos.y - 150, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x + 150, m_tInfo.vPos.y+150, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x - 150, m_tInfo.vPos.y - 150, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x - 150, m_tInfo.vPos.y+ 150, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);


			pMon = CBossSkill2::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 300, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 300, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x - 300, m_tInfo.vPos.y, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x + 300, m_tInfo.vPos.y, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x + 300, m_tInfo.vPos.y - 300, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x + 300, m_tInfo.vPos.y + 300, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x - 300, m_tInfo.vPos.y - 300, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

			pMon = CBossSkill2::Create(m_tInfo.vPos.x - 300, m_tInfo.vPos.y + 300, 0.1);
			m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);


		}

		if (m_fCoolCount >= 18.f&&m_fCoolCount < 23)
		{
			m_eCurState = IDLE;
		}

	


	if (m_bHit == false )
	{
		m_eCurState = HIT;
		CHitEffect* pEffect = CHitEffect::Create(m_tInfo.vPos);
		m_pObjectMgr->AddObject(HITEFFECT, pEffect);

		

		m_fHitTime += m_pTimeMgr->GetDeltaTime();
		if (m_fHitTime >= 0.2)
		{
			CSoundMgr::GetInstance()->PlaySound(L"BOSS_WIND_HIT_2.mp3", CSoundMgr::UI); 
			m_fHitTime = 0;
			m_bHit = true;
		}

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
	}
	if (m_bDieCheck)
	{
		m_eCurState = DIE;
		m_fCoolCount = 25;
		CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney += 7;
		if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney >= 10000)
		{
			CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney = 10000;
		}
	}

	return NO_EVENT;
}

void CBoss::LateUpdate()
{
	
	D3DXMATRIX matScale, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*0.6,
		m_tInfo.vSize.y*0.6, 0.f);
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

void CBoss::Render()
{
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Boss_Right", m_wstStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBoss::Initialize()
{
	m_tInfo.vPictureSize = { 220,300,0 };
	m_tInfo.vPos = { 2500, 52 * 40, 0.f };//2500,1920
										  //CScrollMgr::InitializeScroll(m_tInfo.vPos - D3DXVECTOR3{ WINCX * 0.5f, WINCY * 0.5f, 0 });

	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fSpeed = 3.f;

	m_eCurState = HELLO;
	m_wstStateKey = L"Idle";

	m_fCoolCount = 0;
	m_fCoolCount2 = 0;
	m_bCheck = true;
	m_bHit = true;
	m_bSkill1 = true;
	m_bSkill2 = true;
	m_bSkill3 = true;
	m_bDieCheck = false;
	return S_OK;
}

void CBoss::Release()
{
}



CBoss * CBoss::Create(float _X,float _Y)
{
	CBoss* pInstance = new CBoss;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _X;
	pInstance->GetInfo().vPos.y = _Y;
	return pInstance;
}

void CBoss::StateKey()
{
	switch (m_eCurState)
	{
	case CBoss::IDLE:
		m_wstStateKey = L"Idle";
		m_fMaxFrameCount = 1;
		break;
	case CBoss::ATTACK:
		m_wstStateKey = L"Attack";
		m_fMaxFrameCount = 6;
		break;
	case CBoss::HELLO:
		m_wstStateKey = L"Hello";
		m_fMaxFrameCount = 9;
		break;
	case CBoss::READY:
		m_wstStateKey = L"Ready";
		m_fMaxFrameCount = 2;
		break;
	case CBoss::JUMPSKILL:
		m_wstStateKey = L"JumpSkill";
		m_fMaxFrameCount = 8;
		break;
	case CBoss::HIT:
		m_wstStateKey = L"Hit";
		m_fMaxFrameCount = 2;
		break;
	case CBoss::DIE:
		m_wstStateKey = L"Die";
		m_fMaxFrameCount = 1;
		break;
	default:
		break;
	}
}
