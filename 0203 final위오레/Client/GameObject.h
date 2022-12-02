#pragma once
class CGameObject
{
protected:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
	inline INFO&	GetInfo() { return m_tInfo; }
	inline int Obj_Dead() { return DEAD_OBJ; }
	inline void Set_Hit() { m_bHit = false; }
	inline bool Get_Hit() { return m_bHit; }
	inline void Set_Die() { m_bDie = true; }
	inline bool Get_Die() { return m_bDie ; }
protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CTimeMgr*		m_pTimeMgr;
	CObjectMgr*		m_pObjMgr;

	INFO		m_tInfo;
	bool m_bHit;
	bool m_bDie;

	
};

