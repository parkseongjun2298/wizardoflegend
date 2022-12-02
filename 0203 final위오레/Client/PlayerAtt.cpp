#include "stdafx.h"
#include "PlayerAtt.h"
#include "Player.h"
#include"Mouse.h"
#include"ScrollMgr.h"
#include"TimeMgr.h"
#include"PlayerMp.h"

CPlayerAtt::CPlayerAtt()
	:m_pObjectMgr(CObjectMgr::GetInstance()),
	m_fFrameStart(0.f), m_fMaxFrameCount(0.f), m_fAngle(0.f)
{
	//	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CPlayerAtt::~CPlayerAtt()
{
	Release();
}


int CPlayerAtt::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime()*4;

	if (m_fFrameStart >= m_fMaxFrameCount)
	{
		m_fFrameStart = 0.f;
		return DEAD_OBJ;
	}
	static_cast<CPlayerMp*>(m_pObjectMgr->m_ObjectList[PLAYERMP].front())->GetInfo().vSize.x += 0.0005f;

	return NO_EVENT;
	
}

void CPlayerAtt::LateUpdate()
{
	D3DXMATRIX matScale, matRotZ, matTrans, matPar;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 1.f);
	D3DXMatrixTranslation(&matTrans,   //나오는거리
		m_tInfo.vDir.x * -50.f,
		m_tInfo.vDir.y * -50.f, 0.f);
	
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matPar,
		m_tInfo.vPos.x - CScrollMgr::GetScrollMove().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollMove().y, 0.f);



	m_tInfo.matWorld = matScale * matRotZ * matTrans * matPar;
}

void CPlayerAtt::Render()
{
		
		const TEX_INFO* pTexInfo2 = m_pTextureMgr->GetTexInfo(L"Attack", L"DownR", (int)m_fFrameStart);
		NULL_CHECK(pTexInfo2);
		float fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		 m_pDeviceMgr->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	

}

HRESULT CPlayerAtt::Initialize()
{
	
	m_tInfo.vPictureSize = { 94,91,0 };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vInitialDir = { 0.f, -1.f, 0.f };

	// 항등행렬 함수. (행렬 초기화)
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	
	m_fMaxFrameCount = 12.f;

	bCoolCheck = true;
//	m_fAngle = 5;
	return S_OK;
}

void CPlayerAtt::Release()
{
}

CPlayerAtt * CPlayerAtt::Create(D3DXVECTOR3& vPos, D3DXVECTOR3& Dir, float Angle)
{
	CPlayerAtt* pInstance = new CPlayerAtt;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vDir = Dir;
	pInstance->m_fAngle = Angle;

	return pInstance;
}
