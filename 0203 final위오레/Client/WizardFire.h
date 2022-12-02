#pragma once
#include "GameObject.h"
class CWizardFire :
	public CGameObject
{
private:
	CWizardFire();
public:
	virtual ~CWizardFire();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	enum MONSTER_STATE
	{
		
		ATTACK
		
	};
public:
	static CWizardFire* Create(D3DXVECTOR3& _Pos, D3DXVECTOR3& Dir, float Angle);
private:
	void StateKey();
private:

	
	CObjectMgr*	m_pObjectMgr;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	
	float		m_fCoolTest;
	float			m_fAngle;
	float			m_fCoolTime;
	MONSTER_STATE	m_eCurState;
	float			m_fCoolCount;
	float			m_fSpeed;
	bool			m_bCheck;
	wstring			m_wstStateKey;
	D3DXVECTOR3		m_Dir;


};

