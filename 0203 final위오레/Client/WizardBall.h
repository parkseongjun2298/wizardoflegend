#pragma once
#include "GameObject.h"
class CWizardBall :
	public CGameObject
{
private:
	CWizardBall();
public:
	virtual ~CWizardBall();
public:
	//const INFO& GetInfo() const;

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
	static CWizardBall* Create(D3DXVECTOR3& _Pos, D3DXVECTOR3& Dir, float Angle);

private:
	void StateKey();

private:
	CObjectMgr*	m_pObjectMgr;

	float		m_fFrameStart;
	float		m_fMaxFrameCount;

	float		m_fCoolTest;
	float		m_fAngle;
	float		m_fSpeed;

	float		m_fCoolTime;
	float		m_fCoolCount;

	MONSTER_STATE m_eCurState;
	

	D3DXVECTOR3 m_Dir;
	bool       m_bCheck;
	wstring    m_wstStateKey;
};

