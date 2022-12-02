#include "stdafx.h"
#include "BoxAttack.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"
CBoxAttack::CBoxAttack()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	 m_fSpeed(0),m_fFrameStart(0),m_fMaxFrameCount(0), m_fAngle(0), m_fBoxCool(0), m_fBoxCoolCount(0)
{
	//ZeroMemory(&m_tInfo, sizeof(INFO));
}


CBoxAttack::~CBoxAttack()
{
	Release();
}

int CBoxAttack::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_fMaxFrameCount)
	{
		
		m_fFrameStart = 0.f;
	}
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	//m_Dir = m_PPos - m_tInfo.vPos;

	if (pPlayer->GetInfo().vPos.x < m_tInfo.vPos.x)
		m_tInfo.vSize = { -1.f, 1.f, 0.f };
	else
		m_tInfo.vSize = { 1.f, 1.f, 0.f };
	if (m_bCheck)
	{
		m_fBoxCoolCount += m_pTimeMgr->GetDeltaTime();
		
		if (m_fBoxCool <= m_fBoxCoolCount)
		{ 
			
		D3DXVec3Normalize(&m_Dir, &m_Dir);
		D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);
		
			m_tInfo.vPos += m_Dir*m_fSpeed;
			
			

			if (m_fBoxCool+2<= m_fBoxCoolCount)
			{
				CSoundMgr::GetInstance()->PlaySound(L"ROCK_THROW_1.mp3", CSoundMgr::BOSS);
				
				return DEAD_OBJ;
			}

		
		}
		else
			m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;
	}

		
	

	return NO_EVENT;
}

void CBoxAttack::LateUpdate()
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

void CBoxAttack::Render()
{
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Boss_BoxAttack", m_wstStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBoxAttack::Initialize()
{
	m_tInfo.vPictureSize = { 110,155,0 };
	
										 

	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fSpeed = 18.f;

	m_eCurState = IDLE;
	m_wstStateKey = L"Idle";

	m_bCheck = true;
	m_bHit = true;
	return S_OK;
}

void CBoxAttack::Release()
{
}

CBoxAttack * CBoxAttack::Create(float  _X,float _Y,float _Cool,D3DXVECTOR3 _PPos  )
{
	CBoxAttack* pInstance = new CBoxAttack;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _X;
	pInstance->GetInfo().vPos.y = _Y;
	pInstance->m_fBoxCool = _Cool;
	pInstance->m_PPos = _PPos;
	return pInstance;
}

void CBoxAttack::StateKey()
{
	switch (m_eCurState)
	{
	case CBoxAttack::IDLE:
		m_wstStateKey = L"Idle";
		m_fMaxFrameCount = 1;
		break;
	default:
		break;
	}
}
