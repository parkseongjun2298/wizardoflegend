#pragma once

typedef struct tagPathInfo
{
	wstring wstrObjectKey;
	wstring wstrStateKey;
	wstring wstrRelativePath;
	int iFileCount;
}PATH_INFO;

typedef struct tagUnitInfo
{
	wstring wstrName;
	int iAtt, iDef;
	int iJobIndex, iItem;
}UNIT_INFO;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	BYTE byDrawID;
	BYTE byOption;
	int iMyIndex;
	int iParentIndex;
}TILE_INFO;

typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO	tImgInfo;
}TEX_INFO;

typedef struct tagInfo
{
	//D3DXVECTOR2: 2차원 벡터
	//D3DXVECTOR3: 3차원 벡터
	//D3DXVECTOR4: 3차원 벡터

	D3DXVECTOR3	vPos;			// 위치벡터
	D3DXVECTOR3 vDir;			// 방향벡터
	D3DXVECTOR3 vInitialDir;	// 초기 방향벡터 (고정 축)
	D3DXVECTOR3 vSize;

	D3DXMATRIX matWorld;		// 월드행렬
	D3DXVECTOR3 vPictureSize;

	int iMaxHp;
	int iHP;
	int iMoney;
	int iMaxMp;
	int iMp;
	int iFireCount;
	int iAtt;
	int m_iHitCount;

}INFO;