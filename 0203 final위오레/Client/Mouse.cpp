#include "stdafx.h"
#include "Mouse.h"
#include "ScrollMgr.h"

CMouse::CMouse()
	:m_fFrameStart(0),m_fMaxFrameCount(0)


{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CMouse::~CMouse()
{
}

D3DXVECTOR3 CMouse::GetMousePos()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x + CScrollMgr::GetScrollMove().x, (float)pt.y +CScrollMgr::GetScrollMove().y, 0.f);
}

int CMouse::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
		m_fFrameStart = 0.f;
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	m_tInfo.vPos.x = float(pt.x);
	m_tInfo.vPos.y = float(pt.y);
	
	return NO_EVENT;
}

void CMouse::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x ,
		m_tInfo.vPos.y , 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CMouse::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Ui", L"Mouse", (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CMouse::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f }; 
	m_tInfo.vPictureSize = { 66,63,0 };
	//m_vPictureSize = { 66,63,0 };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };
	m_tInfo.vPos={ 400,300,0 };
	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_fMaxFrameCount = 1.f;
	
	return S_OK; // 초기화 성공
}

void CMouse::Release()
{
}

CMouse * CMouse::Create()
{
	CMouse* pInstance = new CMouse;

	if (FAILED(pInstance->Initialize()))
	{


	//	SafeDelete(pInstance);
		return nullptr;


	}

	return pInstance;
}

