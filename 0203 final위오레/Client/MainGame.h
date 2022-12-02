#pragma once

class CGameObject;
class CMainGame
{
private:
	CMainGame();

public:
	~CMainGame();

public:	
	void Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();
	void RenderFPS();

public:
	static CMainGame* Create();

private:
	
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CKeyMgr*		m_pKeyMgr;
	CObjectMgr*		m_pObjectMgr;
	CTimeMgr*		m_pTimeMgr;
	CSceneMgr*		m_pSceneMgr;
	CSoundMgr*		m_pSoundMgr;

	float			m_fTimeCount;
	int				m_iFPS;
	TCHAR			m_szFPS[MIN_STR];
	TCHAR           m_szMoney[MIN_STR];
	int				m_iEvent;
	
};
