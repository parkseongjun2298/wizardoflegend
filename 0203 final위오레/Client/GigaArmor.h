#pragma once
#include "GameObject.h"
class CGigaArmor :
	public CGameObject
{
private:
	CGigaArmor();
public:
	virtual ~CGigaArmor();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CGigaArmor* Create(D3DXVECTOR3& vPos, D3DXVECTOR3& Dir, float Angle, bool LR, float fFireCount);
private:

	INFO		m_tInfo;
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;

	float		m_fAngle;
	float		m_fAngle2;
	float		m_fCoolTime;

	float		m_fCoolCount;
	float		m_fFireCount;
	bool        m_bCheck;

	bool		m_bIsAngle;
};

