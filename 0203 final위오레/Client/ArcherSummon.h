#pragma once
#include "Effect.h"
class CArcherSummon :
	public CEffect
{
private:
	CArcherSummon();
public:
	virtual ~CArcherSummon();

	// CEffect을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CArcherSummon* Create(CGameObject* pOwner, CEffectIMP* pImp, float fAngle, float Time);

private:
	CObjectMgr*	m_pObjectMgr;
	float		m_fAngle;
	float		m_fCount;
private:
	float	m_fLifeTime;
	float	m_fTimeCount;
	bool    m_bCheck;
	CGameObject* m_pOwner;
};

