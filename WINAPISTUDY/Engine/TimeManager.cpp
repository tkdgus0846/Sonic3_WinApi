#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	m_FPS = 1;
	m_DT = 1.f;
	m_UpdateCount = 0;
	m_UpdateOneSecond = 0.f;

	prevTime = chrono::high_resolution_clock::now();
	curTime = chrono::high_resolution_clock::now();

	dwTime = GetTickCount();
}

void TimeManager::Update()
{
	// 이전프레임에서 현재프레임까지 걸린 시간을 측정
	// 단위시간 = 현재프레임시간 - 이전프레임시간
	curTime = chrono::high_resolution_clock::now();
	chrono::duration<float> elapsed = curTime - prevTime;

	m_DT = elapsed.count();
	if (m_DT > 0.1f) m_DT = 0.1f;
	prevTime = curTime;

	// 1초가 걸릴때까지 반복한 횟수가 초당프레임수
	m_UpdateCount++;
	m_UpdateOneSecond += m_DT;
	if (m_UpdateOneSecond >= 1.0)
	{
		m_FPS = m_UpdateCount;
		m_UpdateOneSecond = 0.f;
		m_UpdateCount = 0;
	}
}

bool TimeManager::RestrictFPS()
{
	if (dwTime + 10 < GetTickCount())
	{
		dwTime = GetTickCount();
		return true;
	}
	return false;
}

INT TimeManager::GetFPS()
{
    return m_FPS;
}

float TimeManager::GetDT()
{
    return m_DT;
}
