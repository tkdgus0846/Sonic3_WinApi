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
	// ���������ӿ��� ���������ӱ��� �ɸ� �ð��� ����
	// �����ð� = ���������ӽð� - ���������ӽð�
	curTime = chrono::high_resolution_clock::now();
	chrono::duration<float> elapsed = curTime - prevTime;

	m_DT = elapsed.count();
	if (m_DT > 0.1f) m_DT = 0.1f;
	prevTime = curTime;

	// 1�ʰ� �ɸ������� �ݺ��� Ƚ���� �ʴ������Ӽ�
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
