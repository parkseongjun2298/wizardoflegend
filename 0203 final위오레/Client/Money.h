#pragma once
#include "GameObject.h"
class CMoney :
	public CGameObject
{
private:
	CMoney();
public:
	virtual ~CMoney();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CMoney* Create();
	
private:
	
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float		m_fAngle;
	TCHAR			m_szMoney[MIN_STR];
};


