#pragma once
#include "Effect.h"
class CGaiaEffect :
	public CEffect
{
private:
	CGaiaEffect();
public:
	virtual ~CGaiaEffect();

	// CEffect을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CGaiaEffect* Create(CEffectIMP* pImp, float fAngle,float Time);

private:
	CObjectMgr*	m_pObjectMgr;
	float		m_fAngle;
	float		m_fCount;
	float		m_fDist;
private:
	float	m_fLifeTime;
	float	m_fTimeCount;
	bool    m_bCheck;
};

