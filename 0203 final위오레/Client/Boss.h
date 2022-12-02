#pragma once
#include "GameObject.h"
class CBoss :
	public CGameObject
{
private:
	CBoss();
public:
	virtual ~CBoss();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBoss* Create(float _X, float _Y);
public:
	inline void Set_DieCheck() { m_bDieCheck = true; }

public:
	enum BOSS_STATE
	{
		IDLE,
		ATTACK,
		HIT,
		DIE,
		HELLO,
		JUMPSKILL,
		READY

	};
	enum BOSS_STATE2
	{
		UP,
		DOWN,
		LEFT,
		RIGHT

	};
	
private:
	void StateKey();
private:


	CObjectMgr*	m_pObjectMgr;

	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float		m_fAngle;
	float		m_fSpeed;
	float		m_fCoolTime;
	float		m_fCoolCount;

	float		m_fCoolTime2;
	float		m_fCoolCount2;
	BOSS_STATE	m_eCurState;
	
	BOSS_STATE2 m_eCurState2;

	D3DXVECTOR3 m_Dir;
	bool       m_bCheck;
	wstring    m_wstStateKey;

	bool		m_bSkill1;
	bool		m_bSkill2;
	bool		m_bSkill3;
	float		 m_fHitTime;
	float		m_fDieCount;
	bool		m_bDieCheck;
	float		m_fSkill2Cool;
};

