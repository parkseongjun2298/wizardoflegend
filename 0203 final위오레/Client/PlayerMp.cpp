#include "stdafx.h"
#include "PlayerMp.h"
#include"Player.h"

CPlayerMp::CPlayerMp()
	:m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0)
{
}


CPlayerMp::~CPlayerMp()
{
}

int CPlayerMp::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	if (m_tInfo.vSize.x >= 0.9)
	{
		pPlayer->Set_Hyper();
		m_tInfo.vSize.x = 0.9;
		
	}


	return NO_EVENT;
}

void CPlayerMp::LateUpdate()
{
	D3DXMATRIX matScale, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*0.6,
		m_tInfo.vSize.y*0.6, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale *matRotZ* matTrans;

	m_tInfo.matWorld = matScale * matTrans;


	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vInitialDir, &matWorld);
}

void CPlayerMp::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Ui", L"Ui_Mp", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.01f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.01f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CPlayerMp::Initialize()
{
	m_tInfo.vSize = { 0.f, 1.f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	m_tInfo.vPos = { 50,55,0 };
	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 1.f;

	return S_OK; // 초기화 성공
}

void CPlayerMp::Release()
{
}

CPlayerMp * CPlayerMp::Create()
{
	CPlayerMp* pInstance = new CPlayerMp;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);

		return nullptr;


	}

	return pInstance;
}
