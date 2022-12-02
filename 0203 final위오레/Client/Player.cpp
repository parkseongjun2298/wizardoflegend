#include "stdafx.h"
#include "Player.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include"TimeMgr.h"
#include "NormalEffect.h"
#include "BuffEffect.h"
#include"GaiaEffect.h"
#include "AnimateIMP.h"
#include "NoAnimateIMP.h"
#include"PlayerAtt.h"
#include"FireBall.h"
#include"IceBlast.h"

#include"PlayerMp.h"
#include"HitEffect.h"
#include"CoolBar.h"
CPlayer::CPlayer()
	: m_pKeyMgr(CKeyMgr::GetInstance()), m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fSpeed(0), m_fCoolTime(0), m_fCoolCount(0), m_fAngle(0), m_fFireCount(0), m_fCoolTimeDash(0), m_fCoolCountDash(0)
	,m_fFrameIdleStart(0.f),m_fMaxFrameIdleCount(0.f)
	, m_fFrameWalkStart(0.f), m_fMaxFrameWalkCount(0.f), m_fFrameDashStart(0.f), m_fMaxFrameDashCount(0.f)
	,m_fFrameAttStart(0.f),m_fMaxFrameAttCount(0.f), m_fHitTime(0), m_fFrameHitStart(0), m_fMaxFrameHitCount(0)
	, m_fFireSum(0)

	


{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	
}



CPlayer::~CPlayer()
{
	Release();
}



int CPlayer::Update()
{
	
	
	KeyInput();

	m_fFireSum += CTimeMgr::GetInstance()->GetDeltaTime();
	//cout << "불꽃생성" << m_fFireSum << endl;
	if (m_fFireSum >= 1)
	{
		++m_tInfo.iFireCount;
		m_fFireSum = 0;
	} 
	if (m_tInfo.iFireCount >= 8)
		m_tInfo.iFireCount = 8;

	// 초당 m_fMaxFrameCount만큼 재생.
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;

	m_fFrameIdleStart += m_fMaxFrameIdleCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameIdleStart >= m_fMaxFrameIdleCount)
		m_fFrameIdleStart = 0.f;

	m_fFrameWalkStart += m_fMaxFrameWalkCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameWalkStart >= m_fMaxFrameWalkCount)
		m_fFrameWalkStart = 0.f;


	m_fFrameDashStart += m_fMaxFrameDashCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameDashStart >= m_fMaxFrameDashCount)
		m_fFrameDashStart = 0.f;

	m_fFrameAttStart += m_fMaxFrameAttCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameAttStart >= m_fMaxFrameAttCount)
		m_fFrameAttStart = 0.f;
	m_fFrameHitStart += m_fMaxFrameHitCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameHitStart >= m_fMaxFrameHitCount)
		m_fFrameHitStart = 0.f;
	
	

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	for (int i = 0; i < 100; ++i)
	{
	D3DXMATRIX matScale, matTrans ,matRotZ;
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

	D3DXVECTOR3 Dir = CMouse::GetMousePos() - m_tInfo.vPos;
	D3DXVec3Normalize(&Dir, &Dir);

	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &Dir, &m_tInfo.vDir);

	
		if (Cross.z < 0)
			m_fAngle += 1;
		else if (Cross.z > 0)
			m_fAngle -= 1;
	}

	

	
	CSoundMgr::GetInstance()->Update();




}

void CPlayer::Render()
{
	
	if(m_eCurState==IDLE&&m_eCurState2==DOWN)
	{ 
		
	const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Front", L"Idle", (int)m_fFrameIdleStart);
	NULL_CHECK(pTexInfo2);
	float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurState == IDLE&&m_eCurState2 == UP)
	{
		const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Back", L"Idle", (int)m_fFrameIdleStart);
		NULL_CHECK(pTexInfo2);
		float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (m_eCurState == IDLE&&m_eCurState2 == LEFT)
	{
		const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Left", L"Idle", (int)m_fFrameIdleStart);
		NULL_CHECK(pTexInfo2);
		float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurState == IDLE&&m_eCurState2 == RIGHT)
	{
		const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Right", L"Idle", (int)m_fFrameIdleStart);
		NULL_CHECK(pTexInfo2);
		float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//========================================================================================================iDle
	if ((m_pKeyMgr->KeyPressing(KEY_LBUTTON)&& m_eCurState==ATTACK&&m_eCurState2 == DOWN)|| (m_pKeyMgr->KeyPressing(KEY_RBUTTON) && m_eCurState == ATTACK&&m_eCurState2 == DOWN))
	{
		
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Front", L"LeftAttack", (int)m_fFrameAttStart);//수정하기
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	if ((m_pKeyMgr->KeyPressing(KEY_LBUTTON) && m_eCurState == ATTACK&&m_eCurState2 == UP )|| (m_pKeyMgr->KeyPressing(KEY_RBUTTON) && m_eCurState == ATTACK&&m_eCurState2 == UP))
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Back", L"Attack", (int)m_fFrameAttStart);//콤보넣어서수정하기
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	if ((m_pKeyMgr->KeyPressing(KEY_LBUTTON) && m_eCurState == ATTACK&&m_eCurState2 == LEFT) || (m_pKeyMgr->KeyPressing(KEY_RBUTTON) && m_eCurState == ATTACK&&m_eCurState2 == LEFT))
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Left", L"Attack", (int)m_fFrameAttStart);//콤보넣어서수정하기
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	if ((m_pKeyMgr->KeyPressing(KEY_LBUTTON) && m_eCurState == ATTACK&&m_eCurState2 == RIGHT) || (m_pKeyMgr->KeyPressing(KEY_RBUTTON) && m_eCurState == ATTACK&&m_eCurState2 == RIGHT))
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Right", L"Attack", (int)m_fFrameAttStart);//콤보넣어서수정하기
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	//========================================================================================================Attack
	if (m_pKeyMgr->KeyPressing(KEY_DOWN) && m_eCurState == WALK&&m_eCurState2 == DOWN)
	{
		const TEX_INFO* pTexInfo3 = m_pTextureMgr->GetTexInfo(L"Front", L"Walk", (int)m_fFrameWalkStart);
		NULL_CHECK(pTexInfo3);

		float fCenterX = pTexInfo3->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo3->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo3->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	if (m_pKeyMgr->KeyPressing(KEY_DOWN) && m_eCurState == DASH&&m_eCurState2 == DOWN)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Front", L"Dash", (int)m_fFrameDashStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	//========================================================================================================정면 대쉬걷기
	if (m_pKeyMgr->KeyPressing(KEY_UP) && m_eCurState == WALK&&m_eCurState2 == UP)
	{
		const TEX_INFO* pTexInfo3 = m_pTextureMgr->GetTexInfo(L"Back", L"Walk", (int)m_fFrameWalkStart);
		NULL_CHECK(pTexInfo3);

		float fCenterX = pTexInfo3->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo3->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo3->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	if (m_pKeyMgr->KeyPressing(KEY_UP) && m_eCurState == DASH&&m_eCurState2 == UP)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Back", L"Dash", (int)m_fFrameDashStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}


	//========================================================================================================뒷면대쉬걷기


	if (m_pKeyMgr->KeyPressing(KEY_LEFT) && m_eCurState == WALK&&m_eCurState2 == LEFT)
	{
		const TEX_INFO* pTexInfo3 = m_pTextureMgr->GetTexInfo(L"Left", L"Walk", (int)m_fFrameWalkStart);
		NULL_CHECK(pTexInfo3);

		float fCenterX = pTexInfo3->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo3->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo3->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	if (m_pKeyMgr->KeyPressing(KEY_LEFT) && m_eCurState == DASH&&m_eCurState2 == LEFT)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Left", L"Dash", (int)m_fFrameDashStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	//========================================================================================================좌측대쉬걷기
	if (m_pKeyMgr->KeyPressing(KEY_RIGHT) && m_eCurState == WALK&&m_eCurState2 == RIGHT)
	{
		const TEX_INFO* pTexInfo3 = m_pTextureMgr->GetTexInfo(L"Right", L"Walk", (int)m_fFrameWalkStart);
		NULL_CHECK(pTexInfo3);

		float fCenterX = pTexInfo3->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo3->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo3->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	if (m_pKeyMgr->KeyPressing(KEY_RIGHT) && m_eCurState == DASH&&m_eCurState2 == RIGHT)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Right", L"Dash", (int)m_fFrameDashStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	
	
	//========================================================================================================우측대쉬걷기

	if (m_bHit == false)
	{
		
		m_eCurState = HIT;
		CHitEffect* pEffect = CHitEffect::Create(m_tInfo.vPos);
		m_pObjectMgr->AddObject(HITEFFECT, pEffect);
		
		
		m_fHitTime += m_pTimeMgr->GetDeltaTime();
		if (m_fHitTime >= 0.2)
		{
			CSoundMgr::GetInstance()->PlaySound(L"PLAYER_HITED_1.mp3", CSoundMgr::PLAYER);
			m_fHitTime = 0;
			m_bHit = true;
		}

	}

	if ( m_eCurState == HIT&&m_eCurState2 == RIGHT)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Right", L"Hit", (int)m_fFrameHitStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	if (m_eCurState == HIT&&m_eCurState2 == LEFT)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Left", L"Hit", (int)m_fFrameHitStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	if (m_eCurState == HIT&&m_eCurState2 == DOWN)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Front", L"Hit", (int)m_fFrameHitStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	if (m_eCurState == HIT&&m_eCurState2 == UP)
	{
		const TEX_INFO* pTexInfo4 = m_pTextureMgr->GetTexInfo(L"Back", L"Hit", (int)m_fFrameHitStart);
		NULL_CHECK(pTexInfo4);

		float fCenterX = pTexInfo4->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo4->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo4->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	swprintf_s(m_szFireCount, L"%d", m_tInfo.iFireCount);

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 110, 555, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);

	// 다이렉트 폰트는 문자열 출력할 때마다 그 때 맞는 텍스처를 만들어서 출력한다. 겁나 느리다.
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(),
		m_szFireCount, /* 출력할 문자열 */
		lstrlen(m_szFireCount), /* 길이 */
		nullptr, /* RECT 포인터 */
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));



	swprintf_s(m_szMoney, L"%d", m_tInfo.iMoney);

	D3DXMATRIX matTrans2;
	D3DXMatrixTranslation(&matTrans2, 390, 555, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans2);

	// 다이렉트 폰트는 문자열 출력할 때마다 그 때 맞는 텍스처를 만들어서 출력한다. 겁나 느리다.
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(),
		m_szMoney, /* 출력할 문자열 */
		lstrlen(m_szMoney), /* 길이 */
		nullptr, /* RECT 포인터 */
		0,
		D3DCOLOR_ARGB(255, 255, 0, 0));



	


}

HRESULT CPlayer::Initialize()
{
	m_tInfo.vPos = { 2600, 1920, 0.f };
	CScrollMgr::InitializeScroll(m_tInfo.vPos - D3DXVECTOR3{WINCX * 0.5f, WINCY * 0.5f, 0});

	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	m_tInfo.vPictureSize={ 80,83,0 };
	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fMaxFrameIdleCount = 1.f;
	m_fMaxFrameCount = 8.f;
	m_fMaxFrameWalkCount = 10.f;
	m_fMaxFrameDashCount = 8.f;
	m_fMaxFrameAttCount = 16.f;
	//m_fMaxFrameAttCount = 2.f;
	m_fSpeed = 5.0;
	m_eCurState = IDLE;
	m_eNextState = IDLE;
	m_eCurState2 = DOWN;

	m_IceBlastCount = 0;
	m_bIsFire = true;
	//m_fFireCount = 8;
	m_tInfo.iFireCount = 8;
	m_fCoolTime = 0.1;
	m_fCoolTimeDash = 1;

	m_bHyper = false;
	bCoolCheck = true;
	m_bHit = true;
	return S_OK; // 초기화 성공
}

void CPlayer::Release()
{
	
}

void CPlayer::KeyInput()
{
	if (m_pKeyMgr->KeyPressing(KEY_LEFT))
	{
	//	CSoundMgr::GetInstance()->PlaySound(L"RUN_1.mp3", CSoundMgr::PLAYER);
		m_eCurState = WALK;
		m_eCurState2 = LEFT;
		m_tInfo.vPos.x -= m_fSpeed;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(-m_fSpeed, 0.f, 0.f));
	}

	if (m_pKeyMgr->KeyPressing(KEY_RIGHT))
	{ 
	//	CSoundMgr::GetInstance()->PlaySound(L"RUN_1.mp3", CSoundMgr::PLAYER);
		m_eCurState = WALK;
		m_eCurState2 = RIGHT;
		m_tInfo.vPos.x += m_fSpeed;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(m_fSpeed, 0.f, 0.f));
	}
	if (m_pKeyMgr->KeyPressing(KEY_UP))
	{
		//CSoundMgr::GetInstance()->PlaySound(L"RUN_1.mp3", CSoundMgr::PLAYER);
		m_eCurState = WALK;
		m_eCurState2 = UP;
		m_tInfo.vPos.y -= m_fSpeed;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, -m_fSpeed, 0.f));
	}
	if (m_pKeyMgr->KeyPressing(KEY_DOWN))
	{ 
		//CSoundMgr::GetInstance()->PlaySound(L"RUN_1.mp3", CSoundMgr::PLAYER);
		m_eCurState = WALK;
		m_eCurState2 = DOWN;
		m_tInfo.vPos.y += m_fSpeed;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, m_fSpeed, 0.f));
	}

	//m_fCoolCountDash += CTimeMgr::GetInstance()->GetDeltaTime();
	if (m_pKeyMgr->KeyPressing(KEY_SPACE) && (m_pKeyMgr->KeyPressing(KEY_LEFT)/*&& bCoolCheck==true*/))//회피  키다운으로고쳐야함
	{
		m_fHitTime += m_pTimeMgr->GetDeltaTime();
		if (m_fHitTime >= 0.1)
		{
			CSoundMgr::GetInstance()->PlaySound(L"DASH_1.mp3", CSoundMgr::PLAYER);
			m_fHitTime = 0;
			m_bHit = true;
		}
		bCoolCheck = false;
		m_eCurState = DASH;
		m_tInfo.vPos.x -= m_fSpeed * 3;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(-m_fSpeed * 3, 0.f, 0.f));
		////쿨타임 주기 실패
		
				
		//if (m_fCoolCountDash >= m_fCoolTimeDash)//이줄 안탐
		//{
		//	m_fCoolCountDash = 0.f;
		//	bCoolCheck = true;
		//}
		
		
	
		
	}
	if (m_pKeyMgr->KeyPressing(KEY_SPACE) && (m_pKeyMgr->KeyPressing(KEY_RIGHT)))//회피
	{
		m_fHitTime += m_pTimeMgr->GetDeltaTime();
		if (m_fHitTime >= 0.1)
		{
			CSoundMgr::GetInstance()->PlaySound(L"DASH_1.mp3", CSoundMgr::PLAYER);
			m_fHitTime = 0;
			m_bHit = true;
		}
		m_eCurState = DASH;
		m_tInfo.vPos.x += m_fSpeed * 3;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(m_fSpeed * 3, 0.f, 0.f));
	}
	if (m_pKeyMgr->KeyPressing(KEY_SPACE) && (m_pKeyMgr->KeyPressing(KEY_UP)))//회피
	{
		m_fHitTime += m_pTimeMgr->GetDeltaTime();
		if (m_fHitTime >= 0.1)
		{
			CSoundMgr::GetInstance()->PlaySound(L"DASH_1.mp3", CSoundMgr::PLAYER);
			m_fHitTime = 0;
			m_bHit = true;
		}
		m_eCurState = DASH;
		m_tInfo.vPos.y -= m_fSpeed * 3;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, -m_fSpeed * 3.f, 0.f));
	}
	if (m_pKeyMgr->KeyPressing(KEY_SPACE) && (m_pKeyMgr->KeyPressing(KEY_DOWN)))//회피
	{
		m_fHitTime += m_pTimeMgr->GetDeltaTime();
		if (m_fHitTime >= 0.1)
		{
			CSoundMgr::GetInstance()->PlaySound(L"DASH_1.mp3", CSoundMgr::PLAYER);
			m_fHitTime = 0;
			m_bHit = true;
		}
		m_eCurState = DASH;
		m_tInfo.vPos.y += m_fSpeed * 3;
		CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, m_fSpeed * 3.f, 0.f));
	}
	if (m_pKeyMgr->KeyDown(KEY_LBUTTON))									//평타  
	{
		CSoundMgr::GetInstance()->PlaySound(L"NORMAL_ATTACK_1.mp3", CSoundMgr::PLAYER);
		m_eCurState = ATTACK;
		
		CPlayerAtt* pEffect = CPlayerAtt::Create(m_tInfo.vPos,m_tInfo.vDir,m_fAngle);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

	}
	if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
	{
		m_eCurState = IDLE;
	}
	if (m_pKeyMgr->KeyUp(KEY_RBUTTON))
	{
		m_eCurState = IDLE;
	}
	if (m_pKeyMgr->KeyUp(KEY_SPACE))
	{
		m_eCurState = IDLE;
	}
	if (m_pKeyMgr->KeyUp(KEY_LEFT))
	{
		m_eCurState = IDLE;
	}
	if (m_pKeyMgr->KeyUp(KEY_RIGHT))
	{
		m_eCurState = IDLE;
	}
	if (m_pKeyMgr->KeyUp(KEY_UP))
	{
		m_eCurState = IDLE;
	}
	if (m_pKeyMgr->KeyUp(KEY_DOWN))
	{
		m_eCurState = IDLE;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////파이어볼
	m_fCoolCount += CTimeMgr::GetInstance()->GetDeltaTime();
	

	if (m_pKeyMgr->KeyPressing(KEY_RBUTTON))//파이어  
	{
		
		m_eCurState = ATTACK;
		// 
		
		//cout << "파이어카운트:" << m_fFireCount << endl;
		//cout<<"쿨카운트:"<< m_fCoolCount << endl;
		
		if (bCoolCheck == true)//이줄 안탐
		{
						
			CSoundMgr::GetInstance()->PlaySound(L"FIRE_DRAGON_1.mp3", CSoundMgr::PLAYER);
			if(m_tInfo.iFireCount>0)
			{ 
				CFireBall* pEffect = CFireBall::Create(m_tInfo.vPos, m_tInfo.vDir, m_fAngle, m_bIsFire, m_fFireCount);
				m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
				m_bIsFire = !m_bIsFire;

				

			}
									
		}

		if (m_fCoolCount < m_fCoolTime)
		{
			bCoolCheck = false;
			
		}

		if (m_fCoolCount >= m_fCoolTime)
		{
			bCoolCheck = true;
			
			m_fCoolCount = 0.f;
			--m_tInfo.iFireCount;

			if (m_tInfo.iFireCount <= 0)
			{ 
				m_tInfo.iFireCount = 0;
			}

		}
		


	}

	if (m_pKeyMgr->KeyPressing(KEY_Q))
	{
		CSoundMgr::GetInstance()->PlaySound(L"ICE_BLAST_1.mp3", CSoundMgr::PLAYER); 
		CIceBlast* pEffect = CIceBlast::Create(m_tInfo.vPos, m_tInfo.vDir, m_fAngle);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);


		CCoolBar* pEffect2 = CCoolBar::Create(156, 550);
		m_pObjectMgr->AddObject(HITEFFECT, pEffect2);

	
	}
	if (m_pKeyMgr->KeyDown(KEY_R))//Gaia Armor
	{
		CSoundMgr::GetInstance()->PlaySound(L"GAIA_ARMOR_START.mp3", CSoundMgr::PLAYER);
		

		CEffectIMP* pImp = CAnimateIMP::Create(L"Effect", L"Armor", 0.f, 12.f);
		NULL_CHECK(pImp);

		CEffectIMP* pImp2 = CAnimateIMP::Create(L"Effect", L"Armor", 1.f, 12.f);
		NULL_CHECK(pImp2);

		CEffectIMP* pImp3 = CAnimateIMP::Create(L"Effect", L"Armor", 2.f, 12.f);
		NULL_CHECK(pImp3);

		CEffectIMP* pImp4 = CAnimateIMP::Create(L"Effect", L"Armor", 3.f, 12.f);
		NULL_CHECK(pImp4);

		CEffectIMP* pImp5 = CAnimateIMP::Create(L"Effect", L"Armor", 4.f, 12.f);
		NULL_CHECK(pImp5);

		CEffectIMP* pImp6 = CAnimateIMP::Create(L"Effect", L"Armor", 5.f, 12.f);
		NULL_CHECK(pImp6);

		CEffectIMP* pImp7 = CAnimateIMP::Create(L"Effect", L"Armor", 6.f, 12.f);
		NULL_CHECK(pImp7);

		CEffectIMP* pImp8 = CAnimateIMP::Create(L"Effect", L"Armor", 7.f, 12.f);
		NULL_CHECK(pImp8);

		CEffectIMP* pImp9 = CAnimateIMP::Create(L"Effect", L"Armor", 8.f, 12.f);
		NULL_CHECK(pImp9);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f); //생존시간
		CEffect* pEffect = CGaiaEffect::Create(pImp9, 0.f,5.f);//이거도생존시간
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
		
		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f);
		 pEffect = CGaiaEffect::Create(pImp2, 45.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
		
		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f);
		pEffect = CGaiaEffect::Create(pImp3, 90.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
		
		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f);
		pEffect = CGaiaEffect::Create(pImp4, 135.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
		
		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f);
		pEffect = CGaiaEffect::Create(pImp5, 180.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
		
		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f);
		pEffect = CGaiaEffect::Create(pImp6, 225.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
		
		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f);
		pEffect = CGaiaEffect::Create(pImp7, 270.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
		
		pImp = CNoAnimateIMP::Create(L"Effect", L"Armor", 5.f);
		pEffect = CGaiaEffect::Create(pImp8, 315.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		CCoolBar* pEffect2 = CCoolBar::Create(232, 550);
		m_pObjectMgr->AddObject(HITEFFECT, pEffect2);
		
	}
	if (m_pKeyMgr->KeyDown(KEY_E) && !m_bHyper)
	{
		CSoundMgr::GetInstance()->PlaySound(L"ICE_KRYSTAL_START.mp3", CSoundMgr::PLAYER);

		CEffectIMP* pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		NULL_CHECK(pImp);
		CEffect* pEffect = CBuffEffect::Create(pImp, 0.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		pEffect = CBuffEffect::Create(pImp, 120.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		pEffect = CBuffEffect::Create(pImp, 240.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		CCoolBar* pEffect2 = CCoolBar::Create(194, 550);
		m_pObjectMgr->AddObject(HITEFFECT, pEffect2);

		
	}
	
	

	if (m_pKeyMgr->KeyDown(KEY_RETURN) && m_bHyper) //KEY_E 로 하면 왜안될까;;
	{
		CSoundMgr::GetInstance()->PlaySound(L"ULT_USE.mp3", CSoundMgr::UI); 
		CSoundMgr::GetInstance()->PlaySound(L"ICE_KRYSTAL_START.mp3", CSoundMgr::PLAYER);
		CEffectIMP* pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		NULL_CHECK(pImp);
		CEffect* pEffect = CBuffEffect::Create(pImp, 0.f, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		pEffect = CBuffEffect::Create(pImp, 60, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		pEffect = CBuffEffect::Create(pImp, 120, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		pEffect = CBuffEffect::Create(pImp, 180, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		pEffect = CBuffEffect::Create(pImp, 240, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		pImp = CNoAnimateIMP::Create(L"Effect", L"Crash", 5.f);
		pEffect = CBuffEffect::Create(pImp, 300, 5.f);
		m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);

		CCoolBar* pEffect2 = CCoolBar::Create(194, 550);
		m_pObjectMgr->AddObject(HITEFFECT, pEffect2);


		static_cast<CPlayerMp*>(m_pObjectMgr->m_ObjectList[PLAYERMP].front())->GetInfo().vSize.x=0;
		m_bHyper = false;
	}
}

CPlayer* CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

