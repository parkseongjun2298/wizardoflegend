#include "stdafx.h"
#include "AstarMgr.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
	: m_iStartIndex(0)
{
}


CAstarMgr::~CAstarMgr()
{
}

void CAstarMgr::StartAstar(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos)
{
	m_OpenList.clear();
	m_CloseList.clear();
	m_BestPath.clear();

	m_iStartIndex = 0;//�÷��̾ ����ִ� terrain ��ǥ
	int iGoalIndex = 0;// ���콺�� Ŭ���� terrain ��ǥ

	if (true == PathFinding(m_iStartIndex, iGoalIndex))
		CreatePath(m_iStartIndex, iGoalIndex);
}

bool CAstarMgr::PathFinding(int iStartIndex, int iGoalIndex)
{
	//



	return false;
}

void CAstarMgr::CreatePath(int iStartIndex, int iGoalIndex)
{

	//������ ã�� m_BestPath �� ��������





}
