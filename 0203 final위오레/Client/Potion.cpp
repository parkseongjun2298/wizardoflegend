#include "stdafx.h"
#include "Potion.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"
#include"PlayerHp.h"

CPotion::CPotion()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fAngle(0.f)
{
}


CPotion::~CPotion()
{
	Release();
}

int CPotion::Update()
{
	if (m_bDie)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CLICK_MENU.mp3", CSoundMgr::UI);
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
		pPlayer->GetInfo().iMoney -= 100;
		static_cast<CPlayerHp*>(m_pObjectMgr->m_ObjectList[PLAYERHP].front())->GetInfo().vSize.x  += 0.1f;
		
		
		return DEAD_OBJ;
	}
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;


	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	m_Dir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	/*if (pPlayer->GetInfo().vPos.x < m_tInfo.vPos.x)
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	else
	m_tInfo.vSize = { -1.f, 1.f, 0.f };*/

	if (m_bCheck)
	{
		D3DXVec3Normalize(&m_Dir, &m_Dir);
		D3DXVec3Dot(&m_tInfo.vInitialDir, &m_Dir);
		m_tInfo.vPos += m_Dir * 1;

	}



	return NO_EVENT;
}

void CPotion::LateUpdate()
{
	D3DXMATRIX matScale, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*0.5,
		m_tInfo.vSize.y*0.5, 0.f);
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

void CPotion::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Shop", L"Potion", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CPotion::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vPictureSize = { 118,155,0 };
	//m_tInfo.vPos = { 3000, 48 * 40, 0.f };//2500,1920
	//CScrollMgr::InitializeScroll(m_tInfo.vPos - D3DXVECTOR3{ WINCX * 0.5f, WINCY * 0.5f, 0 });

	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// ???????? ????. (???? ??????)
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 1;
	m_bHit = true;
	return S_OK;
}

void CPotion::Release()
{
}

CPotion * CPotion::Create(float _X, float _Y)
{
	CPotion* pInstance = new CPotion;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _X;
	pInstance->GetInfo().vPos.y = _Y;
	return pInstance;
}
