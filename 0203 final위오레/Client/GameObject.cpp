#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pObjMgr(CObjectMgr::GetInstance()),
	m_bDie(false)
{
	
}


CGameObject::~CGameObject()
{
}
