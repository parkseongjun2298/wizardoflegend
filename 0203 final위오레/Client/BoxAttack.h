#pragma once
#include "GameObject.h"
class CBoxAttack :
	public CGameObject
{
private:

	CBoxAttack();
public:
	enum BOXATTACK
	{
		IDLE
	};
public:
	virtual ~CBoxAttack();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CBoxAttack* Create(float  _X, float _Y, float _Cool, D3DXVECTOR3 _PPos);
private:
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
	BOXATTACK m_eCurState;

	float		m_fBoxCool;
	float		m_fBoxCoolCount;

};

