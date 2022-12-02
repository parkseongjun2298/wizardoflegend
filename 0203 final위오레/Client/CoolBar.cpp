#include "stdafx.h"
#include "CoolBar.h"


CCoolBar::CCoolBar()
	:m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0)
{
}


CCoolBar::~CCoolBar()
{
}

int CCoolBar::Update()
{


	m_tInfo.vSize.y-=0.01;
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
	{
		
		m_fFrameStart = 0.f;

		return DEAD_OBJ;
	}
	


	return NO_EVENT;
}

void CCoolBar::LateUpdate()
{
	D3DXMATRIX matScale, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
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

void CCoolBar::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Ui", L"CoolTime", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.01f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CCoolBar::Initialize()
{
	m_tInfo.vSize = { 0.5f, 0.5f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	// 항등행렬 함수. (행렬 초기화)
	//m_tInfo.vPos = { 155,550,0 };
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 1.f;

	return S_OK; // 초기화 성공
}

void CCoolBar::Release()
{
}

CCoolBar * CCoolBar::Create(float _X,float _Y)
{
	CCoolBar* pInstance = new CCoolBar;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);

		return nullptr;


	}
	pInstance->GetInfo().vPos.x = _X-2;
	pInstance->GetInfo().vPos.y = _Y+5;

	return pInstance;
}

CCoolBar * CCoolBar::Create()
{
	CCoolBar* pInstance = new CCoolBar;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);

		return nullptr;


	}

	return pInstance;
}
