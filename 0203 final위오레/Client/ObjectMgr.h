#pragma once

class CGameObject;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	CObjectMgr();
	~CObjectMgr();
public:
	CGameObject* GetTerrain();
	CGameObject* GetPlayer();
	list<CGameObject*>& GetMonster();
	CGameObject* GetPlayerAtt();
	CGameObject* GetBoss();
public:
	HRESULT AddObject(OBJECT_TYPE eType, CGameObject* pObject);
	void Update();
	void LateUpdate();
	void Render();

private:
	void Release();

public:
	typedef list<CGameObject*>	OBJECT_LIST;
	OBJECT_LIST	m_ObjectList[OBJECT_END];
};

