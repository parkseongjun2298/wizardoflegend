#pragma once
#include "GameObject.h"
class CWizard :
	public CGameObject
{
private:
	CWizard();
public:
	virtual ~CWizard();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
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
	static CWizard* Create(float _X, float _Y);

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
	float		m_fCreateTime;
	float       m_fDieCount;

	MONSTER_STATE m_eCurState;
	
	float m_fHitTime;
	D3DXVECTOR3 m_Dir;
	bool		m_bCheck;
	bool		m_bPatt;
	bool		m_bFire;
	bool		m_bBall;
	wstring    m_wstStateKey;
};


