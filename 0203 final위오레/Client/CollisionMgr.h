#pragma once
class CGameObject;
class CCollisionMgr
{
public:
	CCollisionMgr();
	virtual ~CCollisionMgr();

	static void CollisionPlayerAtt_N_Monster(list<CGameObject*> rAtt, list<CGameObject*> rMonster);
	static void CollisionMonster_N_PlayerAtt(list<CGameObject*> rMonster, list<CGameObject*> rPlayer);
	static void CollisionMonsterEf_N_PlayerEf(list<CGameObject*> rPlayer, list<CGameObject*> rMonster);
	static void CollisionPlayer_CardEf(list<CGameObject*> rPlayer, list<CGameObject*> rCard);
	static void CollisionPlayer_Object(list<CGameObject*> rPlayer, list<CGameObject*> rObject);
	static void CollisionMonster_Object(list<CGameObject*> rMonster, list<CGameObject*> rObject);
	static void CollisionMouse_Shop(list<CGameObject*> rMouse, list<CGameObject*> rShop);

	static void CollisionPlayerAtt_N_Boss(list<CGameObject*> rAtt, list<CGameObject*> rBoss);
private:
	static bool CheckSphere(INFO& pDst, INFO& pSrc);
	
	static bool CheckSphere(D3DXVECTOR3& pDst, INFO& pSrc);
	
	
	
};

