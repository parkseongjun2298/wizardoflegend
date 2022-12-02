#pragma once
#include "GameObject.h"
class CCoolBar :
	public CGameObject
{
private:
	CCoolBar();
public:
	virtual ~CCoolBar();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CCoolBar* Create(float _X, float _Y);
	static CCoolBar* Create();
private:
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float		m_fAngle;
};

