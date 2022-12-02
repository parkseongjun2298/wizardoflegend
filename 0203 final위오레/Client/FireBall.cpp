#include "stdafx.h"
#include "FireBall.h"
#include "Player.h"
#include"Mouse.h"
#include"ScrollMgr.h"
#include"TimeMgr.h"
#include"PlayerMp.h"
CFireBall::CFireBall()
	:m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fFrameFEStart(0.f), m_fMaxFrameFECount(0.f), m_fAngle(0.f), m_fAngle2(0.f), m_fFireCount(0),
	m_fCoolCount(0.f), m_fCoolTime(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CFireBall::~CFireBall()
{
	Release();
}

int CFireBall::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	static_cast<CPlayerMp*>(m_pObjectMgr->m_ObjectList[PLAYERMP].front())->GetInfo().vSize.x += 0.0005f;
	if(m_bIsAngle)
	m_fAngle2 += 5.f;
	
	if(!m_bIsAngle)
		m_fAngle2 -= 5.f;

	if (m_fAngle2 > 30.f)
		m_bIsAngle = false;

	if (m_fAngle2 < -30.f)
		m_bIsAngle = true;

	
	

	if (m_fFrameStart >= m_fMaxFrameCount)
	{
		m_fFrameStart = 0.f;
		return DEAD_OBJ;
	}
	m_fFrameFEStart += m_fMaxFrameFECount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameFEStart >= m_fMaxFrameFECount)
	{
		m_fFrameStart = 0.f;
		return DEAD_OBJ;
	}

	m_tInfo.vPos -= m_tInfo.vDir * 1000.f * m_pTimeMgr->GetDeltaTime();
	
	
	
	



	
	
	


	return NO_EVENT;
}

void CFireBall::LateUpdate()
{
	
	D3DXMATRIX matScale, matRotZ, matTrans, matPar,matRotZ2;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 1.f);
	D3DXMatrixTranslation(&matTrans,   //나오는거리
		m_tInfo.vDir.x * -50.f,
		m_tInfo.vDir.y * -50.f, 0.f);

	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixRotationZ(&matRotZ2, D3DXToRadian(-m_fAngle2));
	D3DXMatrixTranslation(&matPar,
		m_tInfo.vPos.x - CScrollMgr::GetScrollMove().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollMove().y, 0.f);



	m_tInfo.matWorld = matScale  * matRotZ* matTrans *matRotZ2* matPar;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vInitialDir, &m_tInfo.matWorld);

	
}

void CFireBall::Render()
{
	if(m_tInfo.vDir.x>0)
	{ 
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Effect", L"FireBallL", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	if (m_tInfo.vDir.x<0)
	{
		const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Effect", L"FireBallR", (int)m_fFrameStart);
		NULL_CHECK(pTexInfo2);
		float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}


	
}

HRESULT CFireBall::Initialize()
{
	m_tInfo.vSize = { 0.7f, 0.7f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	m_tInfo.vPictureSize = { 81.9f,125.3f,0.f };
	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_bIsAngle = true;
	m_fMaxFrameCount = 1.f;
	m_fMaxFrameFECount = 40;
	m_fAngle2 = 0;
	m_bCheck = false;
	m_fCoolCount = 0;
	m_fCoolTime = 1;
	
	return S_OK;
}

void CFireBall::Release()
{
}

CFireBall * CFireBall::Create(D3DXVECTOR3 & vPos, D3DXVECTOR3 & Dir, float Angle, bool LR,float fFireCount)
{

	CFireBall* pInstance = new CFireBall;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vDir = Dir;
	pInstance->m_fAngle = Angle;
	pInstance->m_bIsAngle = LR;
	




	return pInstance;
}
