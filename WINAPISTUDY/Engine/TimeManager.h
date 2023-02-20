#pragma once

class TimeManager
{
	DECLARE_SINGLE(TimeManager)

public:
	void Init();
	void Update();
public:
	bool RestrictFPS();
	INT GetFPS();
	float GetDT();
	

private:
	int				m_FPS;
	float			m_DT;

	int				m_UpdateCount;
	float			m_UpdateOneSecond;

	DWORD			dwTime;

	chrono::high_resolution_clock::time_point prevTime;	// ������������ �ð�
	chrono::high_resolution_clock::time_point curTime;	// ������������ �ð�
};

#define TIME	TimeManager::GetInstance()
#define FPS		TimeManager::GetInstance()->GetFPS()
#define DT		TimeManager::GetInstance()->GetDT()