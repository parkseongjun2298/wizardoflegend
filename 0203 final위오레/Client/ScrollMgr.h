#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	static const D3DXVECTOR3& GetScrollMove();

public:
	static void SetScrollMove(const D3DXVECTOR3& vMove);
	static void InitializeScroll(const D3DXVECTOR3& vPos);
private:
	static D3DXVECTOR3 m_vScrollMove;
};

