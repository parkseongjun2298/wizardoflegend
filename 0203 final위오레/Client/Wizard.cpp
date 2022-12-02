#include "stdafx.h"
#include "Wizard.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"
#include"WizardBall.h"
#include"WizardFire.h"

#include "NoAnimateIMP.h"
#include"SummonAni.h"
#include"Effect.h"
#include"WizardBallSummon.h"
#include"HitEffect.h"
CWizard::CWizard()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f),
	m_fAngle(0), m_fSpeed(0), m_fCoolTime(0), m_fCoolCount(0), m_fHitTime(0)
{
}


CWizard::~CWizard()
{
	Release();
}

int CWizard::Update()
{

	if (m_bDie)

	{
		m_eCurState = DIE;
		CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney += 4;
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
		m_tInfo.vPos -= m_Dir*m_fSpeed;
		if (m_Dir != 0)
			m_eCurState = WALK;
	}

	D3DXVECTOR3 Temp = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	if (D3DXVec3Length(&Temp) >= 250.f)
	{
			m_eCurState = ATTACK;
			m_bCheck = false;

			m_fCoolCount+= m_pTimeMgr->GetDeltaTime();
			if (m_fCoolCount >= 1.f)
			{
				m_bPatt = !m_bPatt;
				m_fCoolCount -= 1.f;
				m_bBall = true;
				m_bFire = true;
			}
		
		if(m_bPatt == true)
		{ 			
			
			if(m_bBall)
			{
				CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
				CWizardBall* pMon = CWizardBall::Create(m_tInfo.vPos, m_tInfo.vDir, m_fAngle);
				m_pObjectMgr->AddObject(MONSTEREFFECT, pMon);

				CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardBallSummon", 0.f, 32.f);
				NULL_CHECK_RETURN(pImp, NO_EVENT);

				CEffect* pEffect = CWizardBallSummon::Create(pMon,pImp, 0.f, 1.f);
				m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
				pImp = CNoAnimateIMP::Create(L"Effect", L"WizardBallSummon", 1.f);
				

				m_bBall = false;
			}
			
		}
		//두개 사이 쿨주기


		if (m_bPatt == false)
		{
			if (m_bFire)
			{

				CWizardFire* pMon2 = CWizardFire::Create(m_tInfo.vPos, m_tInfo.vDir, m_fAngle);
				m_pObjectMgr->AddObject(MONSTEREFFECT, pMon2);
				m_bFire=false;

			}
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

	}

	if (!m_bCheck)
		m_fCoolTime += m_pTimeMgr->GetDeltaTime();

	if (m_fCoolTime >= 1.5f)
	{
		m_bCheck = true;
		m_fCoolTime -= 1.5f;
	}



	return NO_EVENT;
}

void CWizard::LateUpdate()
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

void CWizard::Render()
{
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"WizardLeft", m_wstStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CWizard::Initialize()
{
	m_tInfo.vPos = { 3500, 48 * 40, 0.f };//2500,1920
										  //CScrollMgr::InitializeScroll(m_tInfo.vPos - D3DXVECTOR3{ WINCX * 0.5f, WINCY * 0.5f, 0 });

	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	m_tInfo.vPictureSize = { 107,145,0 };
	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fSpeed = 2.f;

	m_eCurState = IDLE;
	m_wstStateKey = L"Idle";
	m_bPatt = true;
	m_bCheck = false;
	m_bFire = true;
	m_bBall = true;
	m_bHit = true;
	return S_OK;
}

void CWizard::Release()
{
}

CWizard * CWizard::Create(float _X,float _Y)
{
	CWizard* pInstance = new CWizard;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _X;
	pInstance->GetInfo().vPos.y = _Y;
	return pInstance;
}

void CWizard::StateKey()
{
	switch (m_eCurState)
	{
	case CWizard::IDLE:
		m_wstStateKey = L"WizardIdle";
		m_fMaxFrameCount = 1;
		break;
	case CWizard::ATTACK:
		m_wstStateKey = L"WizardAttack";
		m_fMaxFrameCount = 4;
		break;
	case CWizard::DASH:
		break;
	case CWizard::HIT:
		m_wstStateKey = L"WizardHit";
		m_fMaxFrameCount = 2;
		break;
	case CWizard::WALK:
		m_wstStateKey = L"WizardWalk";
		m_fMaxFrameCount = 4;
		break;
	case CWizard::DIE:
		m_wstStateKey = L"WizardDie";
		m_fMaxFrameCount = 4;
		break;
	default:
		break;
	}
}
