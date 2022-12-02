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
	//D3DXVECTOR2: 2���� ����
	//D3DXVECTOR3: 3���� ����
	//D3DXVECTOR4: 3���� ����

	D3DXVECTOR3	vPos;			// ��ġ����
	D3DXVECTOR3 vDir;			// ���⺤��
	D3DXVECTOR3 vInitialDir;	// �ʱ� ���⺤�� (���� ��)
	D3DXVECTOR3 vSize;

	D3DXMATRIX matWorld;		// �������
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