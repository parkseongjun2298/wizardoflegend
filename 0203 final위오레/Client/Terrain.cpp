#include "stdafx.h"
#include "Terrain.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include"Player.h"
#include"SwordMan.h"

CTerrain::CTerrain()
	: m_pKeyMgr(CKeyMgr::GetInstance())
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
	// 초당 300씩 움직여라!
	//float fSpeed = 300.f * m_pTimeMgr->GetDeltaTime();

	//system("cls"); 
	//cout << m_pTimeMgr->GetDeltaTime() << endl;
			
	/*if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
		TileChange(CMouse::GetMousePos() + CScrollMgr::GetScrollMove(), 9);*/

		//충돌처리
	TileCollision();
	TileToSwordManCollision();

	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
	
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{

		auto& temp = CScrollMgr::GetScrollMove() - m_vecTile[i]->vPos;
		float fX=abs(temp.x)-TILECX;
		float fY=abs(temp.y)- TILECY;

		if (fX+20 > WINCX||fY+20>WINCY)
			continue;



		pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 
			m_vecTile[i]->vPos.x - CScrollMgr::GetScrollMove().x, 
			m_vecTile[i]->vPos.y - CScrollMgr::GetScrollMove().y,
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

	FAILED_CHECK_RETURN(LoadData(L"../Data/MapData2.dat"), E_FAIL);
	FAILED_CHECK_RETURN(CreateGraph(), E_FAIL);
	return S_OK;
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
}

void CTerrain::TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption)
{
	//int iIndex = GetTileIndex(vPos);

	//if (-1 == iIndex)
	//	return;

	//m_vecTile[iIndex]->byDrawID = byDrawID;
	//m_vecTile[iIndex]->byOption = byOption;
}

void CTerrain::TileCollision( )
{
	//타일충돌
	m_pPlayer =static_cast<CPlayer*>( m_pObjMgr->GetPlayer());
	
	D3DXVECTOR3 vPos = m_pPlayer->GetInfo().vPos;
	
	int iCullX = vPos.x / TILECX;
	int iCullY = vPos.y / TILECY;
	
	int iMinX = iCullX - 1;
	int iMinY = iCullY - 1;
	int iMaxX = iCullX + 2;
	int iMaxY = iCullY + 2;
	
	if (iMinX < 0)
		iMinX = 0;
	if (iMinY < 0)
		iMinY = 0;
	if (iMaxX > TILE_COUNT_X)
		iMaxX = TILE_COUNT_X;
	if (iMaxY > TILE_COUNT_Y)
		iMaxY = TILE_COUNT_Y;
	
	for (int i = iMinY; i < iMaxY; ++i)
	{
		for (int j = iMinX; j < iMaxX; ++j)
		{
			int iIndex = TILE_COUNT_X * i + j;

			D3DXVECTOR3 Dist = m_pPlayer->GetInfo().vPos - m_vecTile[iIndex]->vPos;

			float fLength = D3DXVec3Length(&Dist);
			if (fLength < 50)
			{
				if ( (m_vecTile[iIndex]->byDrawID > 36 && m_vecTile[iIndex]->byDrawID < 158  ))
				{
					if (m_vecTile[iIndex]->vPos.x > m_pPlayer->GetInfo().vPos.x)
					{

						m_pPlayer->GetInfo().vPos.x -= 5;
						CScrollMgr::SetScrollMove(D3DXVECTOR3(-5.f, 0.f, 0.f));
					}
					if (m_vecTile[iIndex]->vPos.x < m_pPlayer->GetInfo().vPos.x)
					{

						m_pPlayer->GetInfo().vPos.x += 5;
						CScrollMgr::SetScrollMove(D3DXVECTOR3(5.f, 0.f, 0.f));
					}
					if (m_vecTile[iIndex]->vPos.y < m_pPlayer->GetInfo().vPos.y)
					{

						m_pPlayer->GetInfo().vPos.y += 5;
						CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, 5.f, 0.f));
					}
					if (m_vecTile[iIndex]->vPos.y > m_pPlayer->GetInfo().vPos.y)
					{

						m_pPlayer->GetInfo().vPos.y -= 5;
						CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, -5.f, 0.f));
					}
				}
			}
		} 
	}

	







}

void CTerrain::TileCollisionMon()
{
}

void CTerrain::TileToSwordManCollision()
{
	//타일충돌
	list<CGameObject*> plstMon = m_pObjMgr->GetMonster();
	

	for (auto& pMon : plstMon)
	{
		D3DXVECTOR3 vPos = pMon->GetInfo().vPos;

		int iCullX = vPos.x / TILECX;
		int iCullY = vPos.y / TILECY;

		int iMinX = iCullX - 1;
		int iMinY = iCullY - 1;
		int iMaxX = iCullX + 2;
		int iMaxY = iCullY + 2;

		if (iMinX < 0)
			iMinX = 0;
		if (iMinY < 0)
			iMinY = 0;
		if (iMaxX > TILE_COUNT_X)
			iMaxX = TILE_COUNT_X;
		if (iMaxY > TILE_COUNT_Y)
			iMaxY = TILE_COUNT_Y;

		for (int i = iMinY; i < iMaxY; ++i)
		{
			for (int j = iMinX; j < iMaxX; ++j)
			{
				int iIndex = TILE_COUNT_X * i + j;

				D3DXVECTOR3 Dist = pMon->GetInfo().vPos - m_vecTile[iIndex]->vPos;

				float fLength = D3DXVec3Length(&Dist);
				if (fLength < 50)
				{
					if ((m_vecTile[iIndex]->byDrawID > 36 && m_vecTile[iIndex]->byDrawID < 158))
					{
						if (m_vecTile[iIndex]->vPos.x > pMon->GetInfo().vPos.x)
						{

							pMon->GetInfo().vPos.x -= 5;

						}
						if (m_vecTile[iIndex]->vPos.x < pMon->GetInfo().vPos.x)
						{

							pMon->GetInfo().vPos.x += 5;

						}
						if (m_vecTile[iIndex]->vPos.y < pMon->GetInfo().vPos.y)
						{

							pMon->GetInfo().vPos.y += 5;

						}
						if (m_vecTile[iIndex]->vPos.y > pMon->GetInfo().vPos.y)
						{

							pMon->GetInfo().vPos.y -= 5;

						}
					}
				}
			}
		}
	}




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

HRESULT CTerrain::CreateGraph()
{
	m_vecGraph.resize(TILE_COUNT_X * TILE_COUNT_Y);

	for (int i = 0; i < TILE_COUNT_Y; ++i)
	{
		for (int j = 0; j < TILE_COUNT_X; ++j)
		{
			int iIndex = i * TILE_COUNT_X + j;

			// 좌상단
			if ((0 != i) && !(0 == j && !(i % 2)))
			{
				if((i % 2) && (0 ==  m_vecTile[iIndex - TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TILE_COUNT_X]);
				else if(!(i % 2) && (0 == m_vecTile[iIndex - (TILE_COUNT_X + 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TILE_COUNT_X + 1)]);
			}

			// 우상단
			if ((0 != i) && !(TILE_COUNT_X - 1 == j && (i % 2)))
			{
				if ((i % 2) && (0 == m_vecTile[iIndex - (TILE_COUNT_X - 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TILE_COUNT_X - 1)]);
				else if (!(i % 2) && (0 == m_vecTile[iIndex - TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TILE_COUNT_X]);
			}

			// 좌하단
			if ((TILE_COUNT_Y - 1 != i) && !(0 == j && !(i % 2)))
			{
				if ((i % 2) && (0 == m_vecTile[iIndex + TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TILE_COUNT_X]);
				else if (!(i % 2) && (0 == m_vecTile[iIndex + (TILE_COUNT_X - 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TILE_COUNT_X - 1)]);
			}

			// 우하단
			if ((TILE_COUNT_Y - 1 != i) && !(TILE_COUNT_X - 1 == j && (i % 2)))
			{
				if ((i % 2) && (0 == m_vecTile[iIndex + (TILE_COUNT_X + 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TILE_COUNT_X + 1)]);
				else if (!(i % 2) && (0 == m_vecTile[iIndex + TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TILE_COUNT_X]);
			}
		}
	}

	return S_OK;
}

CTerrain* CTerrain::Create()
{
	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
