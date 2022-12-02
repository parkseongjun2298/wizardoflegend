#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}
CGameObject* CObjectMgr::GetTerrain()
{
	if (m_ObjectList[TERRAIN].empty())
		return nullptr;

	return m_ObjectList[TERRAIN].front();
}

CGameObject* CObjectMgr::GetPlayer()
{
	if (m_ObjectList[PLAYER].empty())
		return nullptr;

	return m_ObjectList[PLAYER].front();
}
list<CGameObject*>& CObjectMgr::GetMonster()
{
	return m_ObjectList[MONSTER];
}
CGameObject * CObjectMgr::GetPlayerAtt()
{
	if(m_ObjectList[PLAYEREFFECT].empty())
		return nullptr;

	return m_ObjectList[PLAYEREFFECT].front();
}

CGameObject * CObjectMgr::GetBoss()
{
	if (m_ObjectList[BOSS].empty())
		return nullptr;

	return m_ObjectList[BOSS].front();
}

HRESULT CObjectMgr::AddObject(OBJECT_TYPE eType, CGameObject* pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_ObjectList[eType].push_back(pObject);

	return S_OK;
}

void CObjectMgr::Update()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		OBJECT_LIST::iterator iter_begin = m_ObjectList[i].begin();
		OBJECT_LIST::iterator iter_end = m_ObjectList[i].end();

		for (; iter_begin != iter_end;)
		{
			int iEvent = (*iter_begin)->Update();

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(*iter_begin);
				iter_begin = m_ObjectList[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
}

void CObjectMgr::LateUpdate()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		OBJECT_LIST::iterator iter_begin = m_ObjectList[i].begin();
		OBJECT_LIST::iterator iter_end = m_ObjectList[i].end();

		for (; iter_begin != iter_end; ++iter_begin)
			(*iter_begin)->LateUpdate();
	}
	//충돌처리부분
	
	CCollisionMgr::CollisionPlayerAtt_N_Monster(m_ObjectList[PLAYEREFFECT], m_ObjectList[MONSTER]);

	//CCollisionMgr::CollisionPlayerAtt_N_Monster(m_ObjectList[PLAYEREFFECT], m_ObjectList[BOSS]);  //보스는 스탠스100으로 안밀리게

	//CCollisionMgr::CollisionMonster_N_PlayerAtt(m_ObjectList[MONSTER], m_ObjectList[PLAYER]);

	CCollisionMgr::CollisionMonster_N_PlayerAtt(m_ObjectList[MONSTEREFFECT], m_ObjectList[PLAYER]);
	CCollisionMgr::CollisionMonsterEf_N_PlayerEf(m_ObjectList[PLAYEREFFECT], m_ObjectList[MONSTEREFFECT]);
	CCollisionMgr::CollisionPlayer_CardEf(m_ObjectList[PLAYER], m_ObjectList[CARD]);
	CCollisionMgr::CollisionMonster_Object(m_ObjectList[MONSTER], m_ObjectList[OBJECT]);
	CCollisionMgr::CollisionPlayer_Object(m_ObjectList[PLAYER], m_ObjectList[OBJECT]);
	CCollisionMgr::CollisionMouse_Shop(m_ObjectList[MOUSE], m_ObjectList[SHOP]);
	CCollisionMgr::CollisionPlayerAtt_N_Boss(m_ObjectList[PLAYEREFFECT], m_ObjectList[BOSS]);
}

void CObjectMgr::Render()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		OBJECT_LIST::iterator iter_begin = m_ObjectList[i].begin();
		OBJECT_LIST::iterator iter_end = m_ObjectList[i].end();

		for (; iter_begin != iter_end; ++iter_begin)
			(*iter_begin)->Render();
	}
}

void CObjectMgr::Release()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for_each(m_ObjectList[i].begin(), m_ObjectList[i].end(), SafeDelete<CGameObject*>);
		m_ObjectList[i].clear();
	}	
}
