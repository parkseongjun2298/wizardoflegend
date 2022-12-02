#pragma once
#include "GameObject.h"

class CMouse : public CGameObject
{
private:
	CMouse();
	virtual ~CMouse();

public:
	static D3DXVECTOR3 GetMousePos();
	

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;

	virtual void LateUpdate() override;

	virtual void Render() override;



	virtual HRESULT Initialize() override;

	virtual void Release() override;

public:
	static CMouse* Create();
private:
	INFO		m_tInfo;
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
	//D3DXVECTOR3		m_vPictureSize;
};

