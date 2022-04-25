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
	//QueryPerformanceFrequency: cpu의 진동수를 받아오는 함수. 1초당 진동수를 반환
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);
}

void CTimeMgr::SetTime(void)
{
	// 호출이 될 때마다 새로운 시간 얻음
	QueryPerformanceCounter(&m_FrameTime);

	//새로운 시간 - 초기 init 되었을 때의 시간
	if( (m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart )
	{
		QueryPerformanceFrequency(&m_CpuTick);

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	//Update 다음 호출 시까지, LARGE_INTEGER을 진동수로 나누면 Frame 속도 반환됨
	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime.QuadPart = m_FrameTime.QuadPart;
}

void CTimeMgr::Release(void)
{

}
