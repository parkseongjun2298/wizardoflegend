#include "stdafx.h"
#include "MainGame.h"


CMainGame::CMainGame()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance()),
	m_fTimeCount(0.f), m_iFPS(0), m_szFPS(L""),
	m_iEvent(NO_EVENT)
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Update()
{	
	m_pTimeMgr->Update();
	m_pKeyMgr->Update();
	m_iEvent = m_pSceneMgr->Update();
	
}

void CMainGame::LateUpdate()
{
	if (CHANGE_SCENE == m_iEvent)
		return;
	m_pObjectMgr->LateUpdate();
	
}

void CMainGame::Render()
{

	if (CHANGE_SCENE == m_iEvent)
		return;
	m_pDeviceMgr->Render_Begin();

	// 이제부터 이 사이에 오브젝트 렌더링 코드 삽입.

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Ui", L"Ready_Menu", 0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.1f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.1f;

	D3DXMATRIX matTemp;
	D3DXMatrixIdentity(&matTemp);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTemp);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX-80, fCenterY-60, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pObjectMgr->Render();
	RenderFPS();	


	
	m_pDeviceMgr->Render_End();		
}

HRESULT CMainGame::Initialize()
{
	

	
	HRESULT hr2 = m_pDeviceMgr->InitDevice(MODE_WIN); //
	FAILED_CHECK_MSG_RETURN(hr2, L"InitDevice Failed", E_FAIL);

	hr2 = m_pSceneMgr->SceneChange(SCENE_LOGO);
	FAILED_CHECK_MSG_RETURN(hr2, L"Logo Change Failed", E_FAIL);

	CSoundMgr::GetInstance()->Initialize();
	CSoundMgr::GetInstance()->PlayBGM(L"MAIN_MENU_BGM.mp3");
	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.5f);


	// Cube Texture
	hr2 = m_pTextureMgr->LoadTexture(
		m_pDeviceMgr->GetDevice(),
		SINGLE_TEXTURE,
		L"../Texture/Stage/Ui/Ready_Menu/READY_MENU.png",
		L"READY_MENU");
	FAILED_CHECK_MSG_RETURN(hr2, L"Cube Image Create Failed", E_FAIL);


	return S_OK;
}

void CMainGame::Release()
{		
	m_pSoundMgr->DestroyInstance();
	m_pSceneMgr->DestroyInstance();
	//m_pAstarMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();

}

void CMainGame::RenderFPS()
{
	++m_iFPS;

	m_fTimeCount += m_pTimeMgr->GetDeltaTime();

	if (1.f <= m_fTimeCount)
	{
		//swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		m_iFPS = 0;
		m_fTimeCount = 0.f;
	}

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 650, 50.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);

	// 다이렉트 폰트는 문자열 출력할 때마다 그 때 맞는 텍스처를 만들어서 출력한다. 겁나 느리다.
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(),
		m_szFPS, /* 출력할 문자열 */
		lstrlen(m_szFPS), /* 길이 */
		nullptr, /* RECT 포인터 */
		0,
		D3DCOLOR_ARGB(255, 0, 255, 0));
	//=======================================//

	//swprintf_s(m_szMoney, L"%d", 0);

	//D3DXMATRIX matTrans2;
	//D3DXMatrixTranslation(&matTrans2, 390, 555, 0.f);

	//m_pDeviceMgr->GetSprite()->SetTransform(&matTrans2);

	//// 다이렉트 폰트는 문자열 출력할 때마다 그 때 맞는 텍스처를 만들어서 출력한다. 겁나 느리다.
	//m_pDeviceMgr->GetFont()->DrawText(
	//	m_pDeviceMgr->GetSprite(),
	//	m_szMoney, /* 출력할 문자열 */
	//	lstrlen(m_szMoney), /* 길이 */
	//	nullptr, /* RECT 포인터 */
	//	0,
	//	D3DCOLOR_ARGB(255, 255, 0, 0));


}

CMainGame* CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
