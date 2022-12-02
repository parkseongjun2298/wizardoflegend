#pragma once
#include "GameObject.h"
class CBossSkill2 :
	public CGameObject
{
private:
	CBossSkill2();
public:
	virtual ~CBossSkill2();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	enum BOSSSKILL2
	{
		IDLE
	};

public:
	static CBossSkill2* Create(float  _X, float _Y, float _Cool);
	void StateKey();
private:
	CObjectMgr*	m_pObjectMgr;
	D3DXVECTOR3 m_Dir;
	D3DXVECTOR3 m_PPos;

	float		m_fSpeed;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float       m_fAngle;
	bool		m_bCheck;
	wstring    m_wstStateKey;
	BOSSSKILL2	m_eCurState;
	
	float		m_fBoxCool;
	float		m_fBoxCoolCount;
};

