#include "stdafx.h"
#include "CollisionMgr.h"
#include"GameObject.h"
#include"ScrollMgr.h"

#include"PlayerMp.h"

#include"Mouse.h"



CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionPlayerAtt_N_Monster(list<CGameObject*> rAtt, list<CGameObject*> rMonster)
{
	if (rAtt.empty() || rMonster.empty())
		return;
	for (auto& rDestObj : rAtt)
	{
		
		for (auto& rSrcObj : rMonster)
		{
			
 			if (CheckSphere(rDestObj->GetInfo(), rSrcObj->GetInfo()))
			{




				D3DXVECTOR3 Temp;
				Temp = rDestObj->GetInfo().vPos - rSrcObj->GetInfo().vPos;

				rSrcObj->GetInfo().vPos-=Temp/100;


				rSrcObj->Set_Hit();

				rSrcObj->GetInfo().m_iHitCount += 1;
			
			
				

				//cout << rSrcObj->GetInfo().m_iHitCount << endl;



				if(rSrcObj->GetInfo().m_iHitCount>150)
				{ 
				rSrcObj->Set_Die();
				}



				
			}
		}
	}

}

void CCollisionMgr::CollisionMonster_N_PlayerAtt(list<CGameObject*> rMonster, list<CGameObject*> rPlayer)
{
	if (rPlayer.empty() || rMonster.empty())
		return;
	for (auto& rDestObj : rMonster)
	{

		for (auto& rSrcObj : rPlayer)
		{

			if (CheckSphere(rDestObj->GetInfo(), rSrcObj->GetInfo()))
			{
				D3DXVECTOR3 Temp;
				Temp = rSrcObj->GetInfo().vPos - rDestObj->GetInfo().vPos;

				rSrcObj->GetInfo().vPos += Temp / 6;  
			
				CScrollMgr::SetScrollMove(Temp / 6);

			



				rSrcObj->Set_Hit();

				
			}
		}
	}
}

void CCollisionMgr::CollisionMonsterEf_N_PlayerEf(list<CGameObject*> rPlayer, list<CGameObject*> rMonster)
{
	
	for (auto& rDestObj : rPlayer)
	{

		for (auto& rSrcObj : rMonster)
		{

			if (CheckSphere(rDestObj->GetInfo(), rSrcObj->GetInfo()))
			{
				

				rSrcObj->Set_Die();
				
				

				


			}
		}
	}
}

void CCollisionMgr::CollisionPlayer_CardEf(list<CGameObject*> rPlayer, list<CGameObject*> rCard)
{
	for (auto& rDestObj : rPlayer)
	{

		for (auto& rSrcObj : rCard)
		{

			if (CheckSphere(rDestObj->GetInfo(), rSrcObj->GetInfo()))
			{
				
				rSrcObj->Set_Die();
				
			}
		}
	}
}

void CCollisionMgr::CollisionPlayer_Object(list<CGameObject*> rPlayer, list<CGameObject*> rObject)
{
	if (rPlayer.empty() || rObject.empty())
		return;
	for (auto& rDestObj : rPlayer)
	{

		for (auto& rSrcObj : rObject)
		{

			if (CheckSphere(rDestObj->GetInfo(), rSrcObj->GetInfo()))
			{




				D3DXVECTOR3 Temp;
				Temp = rSrcObj->GetInfo().vPos - rDestObj->GetInfo().vPos;

					rDestObj->GetInfo().vPos -= Temp / 6;
					
					CScrollMgr::SetScrollMove(-Temp / 6);

			}
		}
	}
}

void CCollisionMgr::CollisionMonster_Object(list<CGameObject*> rMonster, list<CGameObject*> rObject)
{
	if (rMonster.empty() || rObject.empty())
		return;
	for (auto& rDestObj : rMonster)
	{

		for (auto& rSrcObj : rObject)
		{

			if (CheckSphere(rDestObj->GetInfo(), rSrcObj->GetInfo()))
			{
				
				D3DXVECTOR3 Temp;
				Temp = rSrcObj->GetInfo().vPos - rDestObj->GetInfo().vPos;

				rDestObj->GetInfo().vPos -= Temp / 7;


			}
		}
	}
}

void CCollisionMgr::CollisionMouse_Shop(list<CGameObject*> rMouse, list<CGameObject*> rShop)
{
	for (auto& rDestObj : rMouse)
	{

		for (auto& rSrcObj : rShop)
		{

			if (CheckSphere(static_cast<CMouse*>(rDestObj)->GetMousePos(), rSrcObj->GetInfo()))
			{
				
				rSrcObj->Set_Die();


			}
		}
	}
}

void CCollisionMgr::CollisionPlayerAtt_N_Boss(list<CGameObject*> rAtt, list<CGameObject*> rBoss)
{
	if (rAtt.empty() || rBoss.empty())
		return;
	for (auto& rDestObj : rAtt)
	{

		for (auto& rSrcObj : rBoss)
		{

			if (CheckSphere(rDestObj->GetInfo(), rSrcObj->GetInfo()))
			{
								
				rSrcObj->Set_Hit();

				rSrcObj->GetInfo().m_iHitCount += 1;


				//cout << rSrcObj->GetInfo().m_iHitCount << endl;

				



				
				

			}
		}
	}
}



bool CCollisionMgr::CheckSphere(INFO&  pDst, INFO&  pSrc)
{
	float fx = pDst.vPos.x - pSrc.vPos.x;
	float fy = pDst.vPos.y - pSrc.vPos.y;
	float fDist = sqrtf(fx * fx + fy * fy);

	float fRadiusSum = (pDst.vPictureSize.x/2) + (pSrc.vPictureSize.x/2);

	if (fRadiusSum > fDist)
		return true;

	return false;
}

bool CCollisionMgr::CheckSphere(D3DXVECTOR3 & pDst, INFO & pSrc)
{
	float fx = pDst.x - pSrc.vPos.x;
	float fy = pDst.y - pSrc.vPos.y;
	float fDist = sqrtf(fx * fx + fy * fy);

	float fRadiusSum = (10 / 2) + (pSrc.vPictureSize.x / 2);

	if (fRadiusSum > fDist)
		return true;

	return false;
}
