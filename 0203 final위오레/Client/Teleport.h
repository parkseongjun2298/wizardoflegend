#pragma once
#include "GameObject.h"
class CTeleport :
	public CGameObject
{
private:
	CTeleport();
public:
	virtual ~CTeleport();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;


public:
	static CTeleport* Create(float _X, float _Y);
private:
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float		m_fAngle;
};

