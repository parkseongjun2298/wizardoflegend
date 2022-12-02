#pragma once
#include "GameObject.h"
class CPlayerAtt :
	public CGameObject
{
private:
	CPlayerAtt();
public:

	virtual ~CPlayerAtt();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:

	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CPlayerAtt* Create(D3DXVECTOR3& vPos,D3DXVECTOR3& Dir, float Angle);
private:

	//INFO		m_tInfo;
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	bool        bCoolCheck;
	float		m_fAngle;
};

