#include "stdafx.h"
#include "WizardBall.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"
#include"Wizard.h"

CWizardBall::CWizardBall()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f),
	m_fAngle(0), m_fSpeed(0), m_fCoolTime(0), m_fCoolCount(0)
	, m_fCoolTest(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CWizardBall::~CWizardBall()
{
	Release();
}

int CWizardBall::Update()
{

	if (m_bDie)
	{
		CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney += 1;
		return DEAD_OBJ;
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

		if (!m_bCheck)
		{
			m_fCoolTest += m_pTimeMgr->GetDeltaTime();
			if (m_fCoolTest >= 0.3f)
			{
				D3DXVec3Normalize(&m_Dir, &m_Dir);
				D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);
				m_tInfo.vPos += m_Dir*m_fSpeed;
			}
			else
				m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

			if (m_Dir != 0)
				m_eCurState = IDLE;

		}

		D3DXVECTOR3 Temp = pPlayer->GetInfo().vPos - m_tInfo.vPos;
		if (D3DXVec3Length(&Temp) <= 230.f)
		{
			m_eCurState = ATTACK;
			m_bCheck = false;

		}

		if (!m_bCheck)
			m_fCoolTime += m_pTimeMgr->GetDeltaTime();

		if (m_fCoolTime >= 0.5f)
		{
			m_bCheck = true;
			m_fCoolTime -= 0.5f;
		}



	

	return NO_EVENT;
}

void CWizardBall::LateUpdate()
{
	
	for (int i = 0; i < 100; ++i)
	{
		D3DXMATRIX matScale, matRotZ, matTrans, matPar;
		D3DXMatrixScaling(&matScale,
			m_tInfo.vSize.x,
			m_tInfo.vSize.y, 1.f);
		D3DXMatrixTranslation(&matTrans,   //나오는거리
			m_tInfo.vDir.x * -50.f,
			m_tInfo.vDir.y * -50.f, 0.f);

		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
		D3DXMatrixTranslation(&matPar,
			m_tInfo.vPos.x - CScrollMgr::GetScrollMove().x,
			m_tInfo.vPos.y - CScrollMgr::GetScrollMove().y, 0.f);



		m_tInfo.matWorld = matScale * matRotZ * matTrans * matPar;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
		//m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;
		D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vInitialDir, &m_tInfo.matWorld);
		D3DXVECTOR3 Temp = pPlayer->GetInfo().vPos - m_tInfo.vPos;

		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &Temp, &m_tInfo.vDir);


		if (Cross.z < 0)
			m_fAngle += 1;
		else if (Cross.z > 0)
			m_fAngle -= 1;
	}
}

void CWizardBall::Render()
{
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"WizardBall", m_wstStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CWizardBall::Initialize()
{

	m_tInfo.vPictureSize = { 42,43,0 };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fSpeed = 6.f;

	m_eCurState = IDLE;
	m_wstStateKey = L"Idle";

	m_bCheck = true;
	return S_OK;
}

void CWizardBall::Release()
{
}

CWizardBall * CWizardBall::Create(D3DXVECTOR3& _Pos, D3DXVECTOR3& Dir, float Angle) //angle dir 받기
{
	CWizardBall* pInstance = new CWizardBall;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _Pos;
	pInstance->m_tInfo.vDir = Dir;
	pInstance->m_fAngle = Angle;
	return pInstance;
}

void CWizardBall::StateKey()
{
	switch (m_eCurState)
	{
	case CWizardBall::IDLE:
		m_wstStateKey = L"Idle";
		m_fMaxFrameCount = 1;
		break;
	case CWizardBall::ATTACK:
		m_wstStateKey = L"Attack";
		m_fMaxFrameCount = 4;
		break;

	case CWizardBall::HIT:
		m_wstStateKey = L"Hit";
		m_fMaxFrameCount = 2;
		break;
	
	default:
		break;
	}
}
