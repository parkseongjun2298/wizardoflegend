#pragma once
#include "GameObject.h"
class CBossShadow :
	public CGameObject
{
private:

	CBossShadow();
public:
	enum SHADOW
	{
		IDLE,
		ATTACK
	};
public:
	virtual ~CBossShadow();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CBossShadow* Create(D3DXVECTOR3 _PPos);
private:
	void StateKey();
private:
	CObjectMgr*	m_pObjectMgr;
	D3DXVECTOR3 m_Dir;

	float		m_fSpeed;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float       m_fAngle;
	bool		m_bCheck;
	wstring		m_wstStateKey;
	SHADOW		m_eCurState;

	float m_fCoolCount;
};

