#include "stdafx.h"
#include "ScrollMgr.h"

D3DXVECTOR3 CScrollMgr::m_vScrollMove = {};

CScrollMgr::CScrollMgr()
{
}


CScrollMgr::~CScrollMgr()
{
}

const D3DXVECTOR3& CScrollMgr::GetScrollMove()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_vScrollMove;
}

void CScrollMgr::SetScrollMove(const D3DXVECTOR3& vMove)
{
	m_vScrollMove += vMove;
	
}

void CScrollMgr::InitializeScroll(const D3DXVECTOR3 & vPos)
{
	m_vScrollMove = vPos;
}
