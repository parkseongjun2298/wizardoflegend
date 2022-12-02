#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	: m_fDeltaTime(0.f)
{
	// �ϵ��� ������ �ִ� ���ػ� Ÿ�̸� ���� ������ �Լ�.
	// ���⼭ Ÿ�̸� ���� CPU�� �󸶳� �����ϰ��ִ��� ������ ���̴�.
	QueryPerformanceCounter(&m_OldTime1);
	QueryPerformanceCounter(&m_OldTime2);
	QueryPerformanceCounter(&m_CurTime);

	// CPU�� ���ļ��� ������ �Լ�. 1�� ���� CPU�� �󸶳� �����ϴ°�?
	QueryPerformanceFrequency(&m_CPUTick);
}


CTimeMgr::~CTimeMgr()
{
}

float CTimeMgr::GetDeltaTime()
{
	return m_fDeltaTime; // �� �����Ӵ� �ɸ��� �ð�.
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurTime);

	// �ֱ������� ���ļ� ����.
	if (m_CurTime.QuadPart - m_OldTime2.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OldTime2 = m_CurTime;
	}

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime1.QuadPart) / m_CPUTick.QuadPart;
	m_OldTime1 = m_CurTime;
}
