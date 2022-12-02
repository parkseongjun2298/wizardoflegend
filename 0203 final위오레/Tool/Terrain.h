#pragma once

class CToolView;
class CTerrain
{
private:
	CTerrain();

public:
	~CTerrain();

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);

public:	
	int Update();
	void LateUpdate();
	void Render();
	void MiniRender();

private:
	HRESULT Initialize();
	void Release();

public:
	void TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption = 0);
	HRESULT SaveData(const TCHAR* pFilePath);
	HRESULT LoadData(const TCHAR* pFilePath);

private:
	bool IsPicking(const D3DXVECTOR3& vPos, int iIndex);

public:
	static CTerrain* Create(CToolView* pView);

private:
	vector<TILE_INFO*>	m_vecTile;

	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;
	CToolView*			m_pView;
};

