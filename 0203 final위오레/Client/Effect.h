#pragma once
#include "GameObject.h"

// �߻���

class CEffectIMP;
class CEffect : public CGameObject
{
protected:
	CEffect();

public:
	virtual ~CEffect();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual int Update()		PURE;
	virtual void LateUpdate()	PURE;
	virtual void Render()		PURE;

private:
	virtual HRESULT Initialize() PURE;
	virtual void Release() override;

protected:
	CEffectIMP*	m_pImp;
	
};

