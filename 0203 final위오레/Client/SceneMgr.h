#pragma once

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	CSceneMgr();
	~CSceneMgr();

public:
	HRESULT SceneChange(SCENE_TYPE eType);
	int Update();
	void LateUpdate();
	void Render();

private:
	void Release();

private:
	SCENE_TYPE	m_ePreType;
	CScene*		m_pScene;
};


