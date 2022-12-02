#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "Stage.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_ePreType(SCENE_END),
	m_pScene(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

HRESULT CSceneMgr::SceneChange(SCENE_TYPE eType)
{
	if (m_ePreType != eType)
	{
		SafeDelete(m_pScene);

		switch (eType)
		{
		case SCENE_LOGO:
			m_pScene = CLogo::Create();
			break;
		case SCENE_STAGE:
			m_pScene = CStage::Create();
			break;
		case SCENE_END:
			break;
		default:
			break;
		}

		m_ePreType = eType;
	}

	NULL_CHECK_RETURN(m_pScene, E_FAIL);

	return S_OK;
}

int CSceneMgr::Update()
{
	return m_pScene->Update();
}

void CSceneMgr::LateUpdate()
{
	m_pScene->LateUpdate();
}

void CSceneMgr::Render()
{
	m_pScene->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}
