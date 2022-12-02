#include "stdafx.h"
#include "BossHp.h"
#include"Boss.h"

CBossHp::CBossHp()
	:m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0)
{
}


CBossHp::~CBossHp()
{
}

int CBossHp::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;

	CBoss* pPlayer = dynamic_cast<CBoss*>(m_pObjectMgr->GetBoss());

	if (pPlayer->Get_Hit() == false )
	{
		m_tInfo.vSize.x -= 0.0014;
	}

	if (m_tInfo.vSize.x <= 0)
	{
		
		m_tInfo.vSize.x = 0;
		pPlayer->Set_DieCheck();
	}

	return NO_EVENT;
}

void CBossHp::LateUpdate()
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

void CBossHp::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Boss", L"Boss_Hp", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.01f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.01f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBossHp::Initialize()
{
	m_tInfo.vSize = { 0.83f, 1.f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	m_tInfo.vPos = { 310,48,0 };
	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 1.f;

	return S_OK; // 초기화 성공
}

void CBossHp::Release()
{
}

CBossHp * CBossHp::Create()
{
	CBossHp* pInstance = new CBossHp;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);

		return nullptr;


	}

	return pInstance;
}
