#pragma once
#include "GameObject.h"
class CSummonEffect :
	public CGameObject
{
public:
	CSummonEffect();
	virtual ~CSummonEffect();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;
};

