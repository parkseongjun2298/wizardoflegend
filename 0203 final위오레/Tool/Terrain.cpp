#include "stdafx.h"
#include "Terrain.h"
#include "ToolView.h"

CTerrain::CTerrain()	
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pView(nullptr)
{
}


CTerrain::~CTerrain()
{
	Release();
}

int CTerrain::GetTileIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (IsPicking(vPos, (int)i))
			return (int)i;
	}

	return -1;
}

int CTerrain::Update()
{
	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	TCHAR szIndex[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 
			m_vecTile[i]->vPos.x - m_pView->GetScrollPos(0),
			m_vecTile[i]->vPos.y - m_pView->GetScrollPos(1),
			0.f);	

		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(
			pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szIndex, L"%d", i);
		m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(), szIndex, lstrlen(szIndex),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::MiniRender()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;	

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.f);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x * 0.3f,
			m_vecTile[i]->vPos.y * 0.3f,
			0.f);		

		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(
			pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));		
	}
}

HRESULT CTerrain::Initialize()
{
	m_vecTile.reserve(TILE_COUNT_X * TILE_COUNT_Y);

	TILE_INFO* pTile = nullptr;
	float fX = 0.f, fY = 0.f;

	for (int i = 0; i < TILE_COUNT_Y; ++i)
	{
		for (int j = 0; j < TILE_COUNT_X; ++j)
		{
			/*fX = j * TILECX + (i % 2) * (TILECX * 0.5f);
			fY = i * (TILECY * 0.5f);*/
			fX = float((TILECX * j) + (TILECX >> 1));
			fY = float((TILECY * i) + (TILECY >> 1));
			pTile = new TILE_INFO;
			pTile->vPos = { fX, fY, 0.f };
			pTile->byDrawID = 36;
			pTile->byOption = 0;
			pTile->iMyIndex = i * TILE_COUNT_X + j;
			pTile->iParentIndex = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
}

void CTerrain::TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption)
{
	int iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = byOption;

	
}

HRESULT CTerrain::SaveData(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
		WriteFile(hFile, m_vecTile[i], sizeof(TILE_INFO), &dwBytes, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CTerrain::LoadData(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(),
			[](auto& pTile)
		{
			if (pTile)
			{
				delete pTile;
				pTile = nullptr;
			}
		});

		m_vecTile.clear();
	}

	DWORD dwBytes = 0;
	TILE_INFO tInfo = {};	

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(TILE_INFO), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		m_vecTile.push_back(new TILE_INFO(tInfo));
	}

	CloseHandle(hFile);

	return S_OK;
}

bool CTerrain::IsPicking(const D3DXVECTOR3& vPos, int iIndex)
{
	// 내적을 이용한 방식 (데카르트 기준)

	// 12시, 3시, 6시, 9시 순으로 점 4개를 구한다. 
	D3DXVECTOR3 vPoint[4] =
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f },
		{ m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f },
		{ m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f }
	};

	// 4개의 방향벡터를 구함.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 위에서 구한 방향벡터들의 각 법선벡터를 구함.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);

	// 마우스 위치와 4개의 vPoint와의 방향벡터를 구함.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vNormal과 vMouseDir를 내적해서 모두 음수(둔각)가 나온다면 true
	for (int i = 0; i < 4; ++i)
	{
		// 하나라도 양수(예각)가 나오면 false
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;



	///////////////////////////////////////////////////////////////////////


	//// 직선의 방정식 (데카르트 기준)
	//// 0 = ax + b - y: 직선(a, b)가 점(x, y)를 지나간다. 
	//// 0 > ax + b - y: 점(x, y)가 직선(a, b)보다 위에 있다.
	//// 0 < ax + b - y: 점(x, y)가 직선(a, b)보다 아래에 있다.

	//// 기울기 4개
	//float fGradient[4] = 
	//{
	//	(TILECY * 0.5f) / (TILECX * 0.5f),
	//	-(TILECY * 0.5f) / (TILECX * 0.5f),
	//	(TILECY * 0.5f) / (TILECX * 0.5f),
	//	-(TILECY * 0.5f) / (TILECX * 0.5f)
	//};

	//// 4개의 직선이 각각 통과하는 점 4개를 구한다. 12시, 3시, 6시, 9시 순.
	//D3DXVECTOR3 vPoint[4] = 
	//{
	//	{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f },
	//	{ m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f },
	//	{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f },
	//	{ m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f }
	//};

	//// y절편
	//// 0 = ax + b - y
	//// b = -ax + y
	//float fB[4] =
	//{
	//	-(fGradient[0] * vPoint[0].x) + vPoint[0].y,
	//	-(fGradient[1] * vPoint[1].x) + vPoint[1].y,
	//	-(fGradient[2] * vPoint[2].x) + vPoint[2].y,
	//	-(fGradient[3] * vPoint[3].x) + vPoint[3].y
	//};

	//// 0 = ax + b - y: 직선(a, b)가 점(x, y)를 지나간다. 
	//// 0 > ax + b - y: 점(x, y)가 직선(a, b)보다 위에 있다.
	//// 0 < ax + b - y: 점(x, y)가 직선(a, b)보다 아래에 있다.
	//return (0 < fGradient[0] * vPos.x + fB[0] - vPos.y &&
	//	0 < fGradient[1] * vPos.x + fB[1] - vPos.y &&
	//	0 > fGradient[2] * vPos.x + fB[2] - vPos.y &&
	//	0 > fGradient[3] * vPos.x + fB[3] - vPos.y);
}

CTerrain* CTerrain::Create(CToolView* pView)
{
	NULL_CHECK_RETURN(pView, nullptr);
	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_pView = pView;

	return pInstance;
}
