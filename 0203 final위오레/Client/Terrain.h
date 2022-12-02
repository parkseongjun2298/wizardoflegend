#pragma once
#include "GameObject.h"
class CPlayer;
class CSwordMan;
class CTerrain : public CGameObject
{
private:
	CTerrain();

public:
	virtual ~CTerrain();

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);
private:
//	void KeyInput();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	void TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption = 0);
	void TileCollision();
	void TileCollisionMon();
	void TileToSwordManCollision();

private:
	bool IsPicking(const D3DXVECTOR3& vPos, int iIndex);
	HRESULT LoadData(const TCHAR* pFilePath);
	HRESULT CreateGraph();

public:
	static CTerrain* Create();
	
private:
	CKeyMgr*	m_pKeyMgr;	
	list<CGameObject*>			m_listObj;
	vector<TILE_INFO*>			m_vecTile;
	vector<list<TILE_INFO*>>	m_vecGraph;
	CPlayer*					m_pPlayer;
	
};

