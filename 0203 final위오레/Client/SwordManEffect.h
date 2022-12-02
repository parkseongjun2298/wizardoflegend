#pragma once
#include "GameObject.h"
class CSwordManEffect :
	public CGameObject
{
private:
	CSwordManEffect();
public:
	virtual ~CSwordManEffect();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CSwordManEffect* Create(D3DXVECTOR3& vPos, D3DXVECTOR3& Dir, float Angle);
private:

	D3DXVECTOR3 m_Dir;
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	bool        bCoolCheck;
	float		m_fAngle;
};

