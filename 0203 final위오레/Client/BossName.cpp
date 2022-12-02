#include "stdafx.h"
#include "BossName.h"


CBossName::CBossName()
	:m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0)
{
}


CBossName::~CBossName()
{
}

int CBossName::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;


	return NO_EVENT;
}

void CBossName::LateUpdate()
{
	D3DXMATRIX matScale, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*0.5,
		m_tInfo.vSize.y*0.5, 0.f);
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

void CBossName::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Boss", L"Boss_Name", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBossName::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	m_tInfo.vPos = { 400,50,0 };
	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 1.f;

	return S_OK; // 초기화 성공
}

void CBossName::Release()
{
}

CBossName * CBossName::Create()
{
	CBossName* pInstance = new CBossName;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);

		return nullptr;


	}

	return pInstance;
}
