#pragma once
class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	void StartAstar(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos);

private:
	bool PathFinding(int iStartIndex, int iGoalIndex);
	void CreatePath(int iStartIndex, int iGoalIndex);

private:
	list<int>			m_OpenList;		// �̹湮 ������ �ε��� ����.
	list<int>			m_CloseList;	// �湮�ߴ� ������ �ε��� ����.
	list<TILE_INFO*>	m_BestPath;		// ���̽�Ÿ�� ���ؼ� ���� ���.

	int					m_iStartIndex;
};

