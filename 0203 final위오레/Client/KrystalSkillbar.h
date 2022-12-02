#pragma once
#include "GameObject.h"
class CKrystalSkillbar :
	public CGameObject
{
private:
	CKrystalSkillbar();
public:
	virtual ~CKrystalSkillbar();

	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CKrystalSkillbar* Create();
private:
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	float		m_fAngle;
};

