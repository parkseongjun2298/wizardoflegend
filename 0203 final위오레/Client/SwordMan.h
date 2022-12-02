#pragma once
#include "GameObject.h"
class CSwordMan :
	public CGameObject
{
private:

	CSwordMan();
public:
	virtual ~CSwordMan();

public:
	
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;


	enum MONSTER_STATE
	{
		IDLE,
		ATTACK,
		DASH,
		HIT,
		WALK,
		DIE
	};

public:
	static CSwordMan* Create(float _X, float _Y);
	

private:
	void StateKey();

private:
	CObjectMgr*	m_pObjectMgr;

	float		m_fFrameStart;
	float		m_fMaxFrameCount;

	float		m_fFrameIdleStart;
	float		m_fMaxFrameIdleCount;

	float		m_fFrameAttackStart;
	float		m_fMaxFrameAttackCount;
	float		m_fAngle;
	float		m_fSpeed;

	float		m_fCoolTime;
	float		m_fCoolCount;

	float		m_fDieCount;

	float		m_fHitTime;
	MONSTER_STATE m_eCurState;
	MONSTER_STATE m_eNextState;

	D3DXVECTOR3 m_Dir;
	bool       m_bCheck;
	wstring    m_wstStateKey;

	


};

