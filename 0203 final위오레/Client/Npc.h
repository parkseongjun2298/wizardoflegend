#pragma once
#include "GameObject.h"
class CNpc :
	public CGameObject
{
private:
	CNpc();
public:
	virtual ~CNpc();

	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;


public:
	static CNpc* Create(float _X, float _Y);

private:
	CObjectMgr*	m_pObjectMgr;

	float		m_fAngle;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;

	D3DXVECTOR3 m_Dir;
	bool       m_bCheck;
};

