#include "stdafx.h"
#include "Logo.h"


CLogo::CLogo()
	: m_hLoadingThread(nullptr)
{
}


CLogo::~CLogo()
{
	Release();
}

int CLogo::Update()
{
	if (m_pKeyMgr->KeyDown(KEY_RETURN))
	{
		WaitForSingleObject(m_hLoadingThread, INFINITE);

		DWORD dwExitCode = 0;
		GetExitCodeThread(m_hLoadingThread, &dwExitCode); // 쓰레드의 종료코드를 얻어오는 함수.

		if (LOAD_FAIL == dwExitCode)
			return LOAD_FAIL;

		m_pSceneMgr->SceneChange(SCENE_STAGE);
		return CHANGE_SCENE;
	}

	return NO_EVENT;
}

void CLogo::LateUpdate()
{
}

void CLogo::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Ui", L"Ready_Menu", 0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matTemp;
	D3DXMatrixIdentity(&matTemp);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTemp);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));




	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 300.f, 300.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(),
		m_pTextureMgr->GetLoadingMessage().c_str(),
		m_pTextureMgr->GetLoadingMessage().length(),
		nullptr, 0, D3DCOLOR_ARGB(255, 0, 255, 0));
}

HRESULT CLogo::Initialize()
{
	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingFunc, this, 0, nullptr);
	NULL_CHECK_RETURN(m_hLoadingThread, E_FAIL);

	InitializeCriticalSection(&m_Critical);
	//CSoundMgr::GetInstance()->PlayBGM(L"MAIN_MENU_BGM.mp3");
	return S_OK;
}

void CLogo::Release()
{
	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_Critical);
}

CLogo* CLogo::Create()
{
	CLogo* pInstance = new CLogo;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

unsigned CLogo::LoadingFunc(void* pParam)
{
	CLogo* pLogo = reinterpret_cast<CLogo*>(pParam);
	NULL_CHECK_RETURN(pLogo, LOAD_FAIL);

	EnterCriticalSection(&pLogo->m_Critical);

	HRESULT hr = CTextureMgr::GetInstance()->LoadFromPathInfoFile(
		CDeviceMgr::GetInstance()->GetDevice(),
		L"../Data/PathInfo.txt");

	if (FAILED(hr))
	{
		LeaveCriticalSection(&pLogo->m_Critical);

		::MessageBox(0, L"LoadFromPathInfoFile Failed", L"System Error", MB_OK);
		return LOAD_FAIL;
	}

	CTextureMgr::GetInstance()->SetLoadingMessage(L"로딩완료!");

	LeaveCriticalSection(&pLogo->m_Critical);

	return 0;
}
