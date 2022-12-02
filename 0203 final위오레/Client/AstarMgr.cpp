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

	m_iStartIndex = 0;//플레이어가 밟고있는 terrain 좌표
	int iGoalIndex = 0;// 마우스로 클릭한 terrain 좌표

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

	//위에서 찾은 m_BestPath 로 움직여라





}
