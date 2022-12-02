#pragma once
#include "EffectIMP.h"
class CSummonAni :
	public CEffectIMP
{
public:
	CSummonAni();
	virtual ~CSummonAni();
public:
	// CEffectIMP을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX & matWorld) override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CSummonAni* Create(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const float& fFrameStart,
		const float& fMaxFrameCount);

private:
	float		m_fFrameStart;
	float		m_fMaxFrameCount;

	float	m_fLifeTime;
	float	m_fTimeCount;
};

