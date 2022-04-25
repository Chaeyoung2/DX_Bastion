#include "StdAfx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr);

CTimeMgr::CTimeMgr(void)
{
}

CTimeMgr::~CTimeMgr(void)
{
	Release();
}

float CTimeMgr::GetTime(void)
{
	return m_fTime;
}

void CTimeMgr::InitTime(void)
{
	//QueryPerformanceFrequency: cpu�� �������� �޾ƿ��� �Լ�. 1�ʴ� �������� ��ȯ
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);
}

void CTimeMgr::SetTime(void)
{
	// ȣ���� �� ������ ���ο� �ð� ����
	QueryPerformanceCounter(&m_FrameTime);

	//���ο� �ð� - �ʱ� init �Ǿ��� ���� �ð�
	if( (m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart )
	{
		QueryPerformanceFrequency(&m_CpuTick);

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	//Update ���� ȣ�� �ñ���, LARGE_INTEGER�� �������� ������ Frame �ӵ� ��ȯ��
	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime.QuadPart = m_FrameTime.QuadPart;
}

void CTimeMgr::Release(void)
{

}
