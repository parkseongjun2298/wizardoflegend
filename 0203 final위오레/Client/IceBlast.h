#pragma once
#include "GameObject.h"
class CIceBlast :
	public CGameObject
{
private:
	CIceBlast();
public:
	virtual ~CIceBlast();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CIceBlast* Create(D3DXVECTOR3& vPos, D3DXVECTOR3& Dir, float Angle);
private:

	
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	

	float		m_fAngle;
	
	float		m_fCoolTime;

	float		m_fCoolCount;
	
	bool        m_bCheck;

	


};

