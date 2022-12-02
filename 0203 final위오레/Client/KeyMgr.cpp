#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwCurrentKey(0), m_dwKeyDown(0), m_dwKeyUp(0)
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::Update()
{
	m_dwCurrentKey = 0;

	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurrentKey |= KEY_UP;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurrentKey |= KEY_DOWN;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurrentKey |= KEY_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurrentKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurrentKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurrentKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurrentKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurrentKey |= KEY_RBUTTON;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwCurrentKey |= KEY_Q;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurrentKey |= KEY_R;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwCurrentKey |= KEY_E;

}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 전에 누른적 없고 현재 눌렸을 때
	if (!(m_dwKeyDown & dwKey) && (m_dwCurrentKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}

	// 전에 누른적 있고 현재 누르지 않았을 때
	if ((m_dwKeyDown & dwKey) && !(m_dwCurrentKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// 전에 누른적 있고 현재 누르지 않았을 때
	if ((m_dwKeyUp & dwKey) && !(m_dwCurrentKey & dwKey))
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}

	// 전에 누른적 없고 현재 눌렀을 때
	if (!(m_dwKeyUp & dwKey) && (m_dwCurrentKey & dwKey))
	{
		m_dwKeyUp |= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurrentKey & dwKey)
		return true;

	return false;
}
