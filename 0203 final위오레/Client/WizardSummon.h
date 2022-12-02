#pragma once
#include "Effect.h"
class CWizardSummon :
	public CEffect
{
private:
	CWizardSummon();
public:

	virtual ~CWizardSummon();

	// CEffect��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CWizardSummon* Create(CGameObject* pOwner, CEffectIMP* pImp, float fAngle, float Time);

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

