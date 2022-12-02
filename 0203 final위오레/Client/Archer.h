#pragma once
#include "GameObject.h"
class CArcher :
	public CGameObject
{
private:
	CArcher();
public:
	virtual ~CArcher();



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
		HIT,
		WALK,
		DIE
	};

public:
	static CArcher* Create(float _X, float _Y);

private:
	void StateKey();

private:
	CObjectMgr*	m_pObjectMgr;

	float		m_fFrameStart;
	float		m_fMaxFrameCount;

	float		m_fAngle;
	float		m_fSpeed;
	float		m_fHitTime;
	float		m_fCoolTime;
	float       m_fDieCount;

	MONSTER_STATE m_eCurState;
	

	D3DXVECTOR3 m_Dir;
	bool       m_bCheck;
	bool		m_bBowCheck;
	wstring    m_wstStateKey;
};


