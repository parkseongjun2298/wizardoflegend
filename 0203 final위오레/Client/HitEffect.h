#pragma once
#include "GameObject.h"
class CHitEffect :
	public CGameObject
{
private:
	CHitEffect();
public:
	virtual ~CHitEffect();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CHitEffect* Create(D3DXVECTOR3 _Pos);

private:
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	bool        bCoolCheck;
	float		m_fAngle;

	bool		m_bCheck;
};

