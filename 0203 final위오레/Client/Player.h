#pragma once

#include "GameObject.h"

class CPlayer : public CGameObject
{
private:
	CPlayer();

public:
	virtual ~CPlayer();
public:
	inline void Set_Hyper() { m_bHyper=true; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
private:
	void KeyInput();
public:
	static CPlayer* Create();
	
public:
	enum PLAYER_STATE
	{
		IDLE,
		ATTACK,
		DASH,
		HIT,
		WALK

	};
	enum PLAYER_STATE2
	{
		UP,
		DOWN,
		LEFT,
		RIGHT

	};
private:
		
	CKeyMgr*	m_pKeyMgr;
	
	CObjectMgr*	m_pObjectMgr;

	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float		m_fFrameIdleStart;
	float		m_fMaxFrameIdleCount;
	float		m_fFrameWalkStart;
	float		m_fMaxFrameWalkCount;
	float		m_fFrameDashStart;
	float		m_fMaxFrameDashCount;
	float		m_fFrameAttStart;
	float		m_fMaxFrameAttCount;
	float		m_fFrameHitStart;
	float		m_fMaxFrameHitCount;
	float		m_fAngle;
	float		m_fSpeed;
	float		m_fCoolTime;
	float		m_fCoolCount;
	float		m_IceBlastCount;


	bool        bCoolCheck;
	bool		m_bIsFire;
	bool		m_bStage1;
	bool		m_bStage2;
	bool		m_bStage3;

	float	    m_fFireSum;
	float		m_fFireCount;
	float		m_fCoolTimeDash;
	float		m_fCoolCountDash;
	float		m_fSummonCount;
	float		m_fHitTime;
	PLAYER_STATE m_eCurState;
	PLAYER_STATE m_eNextState;

	PLAYER_STATE2 m_eCurState2;
	PLAYER_STATE2 m_eNextState2;
	

	TCHAR           m_szMoney[MIN_STR];
	TCHAR           m_szFireCount[MIN_STR];
	bool m_bHyper;
	
};

