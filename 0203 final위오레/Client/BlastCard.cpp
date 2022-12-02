#include "stdafx.h"
#include "BlastCard.h"
#include "ScrollMgr.h"
#include "Player.h"
#include"TimeMgr.h"
#include"BlastSkillbar.h"
CBlastCard::CBlastCard()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fAngle(0.f)
{
}


CBlastCard::~CBlastCard()
{
	Release();
}

int CBlastCard::Update()
{
	if (m_bDie)
	{
		if (m_BlastUi)
		{
			CBlastSkillbar* pFireCard = CBlastSkillbar::Create();
			m_pObjectMgr->AddObject(UI, pFireCard);
			m_BlastUi = false;
		}
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

void CBlastCard::LateUpdate()
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

void CBlastCard::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Ui", L"ICE_BLAST_CARD", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBlastCard::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vPictureSize = { 128,192,0 };
	//m_tInfo.vPos = { 3000, 48 * 40, 0.f };//2500,1920
	//CScrollMgr::InitializeScroll(m_tInfo.vPos - D3DXVECTOR3{ WINCX * 0.5f, WINCY * 0.5f, 0 });

	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 1;
	m_bHit = true;
	m_BlastUi = true;
	return S_OK;
}

void CBlastCard::Release()
{
}

CBlastCard * CBlastCard::Create(float _X, float _Y)
{
	CBlastCard* pInstance = new CBlastCard;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->GetInfo().vPos.x = _X;
	pInstance->GetInfo().vPos.y = _Y;
	return pInstance;
}
