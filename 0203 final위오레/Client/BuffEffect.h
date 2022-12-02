#pragma once
#include "Effect.h"
class CBuffEffect :
	public CEffect
{
private:
	CBuffEffect();

public:
	virtual ~CBuffEffect();

public:
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render()		override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBuffEffect* Create(CEffectIMP* pImp, float fAngle, float fTime);

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

