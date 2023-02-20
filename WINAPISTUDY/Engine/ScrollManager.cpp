#include "pch.h"
#include "ScrollManager.h"
#include "SoundManager.h"

void ScrollManager::Init()
{
	m_fScrollX = 0.f;
	m_fScrollY = 0.f;

	m_bScrollLock = false;
	m_bScrollShake = false;
	m_bScrollIntenseShake = false;

	m_dwShakeSpeed = 300;
	m_dwIntenseShakeSpeed = 100;
	m_Shake = 1;

	m_bAutoScroll = false;
	m_bIntenseShakeFinished = false;
}

void ScrollManager::Update()
{
	if (m_bAutoScroll)
	{
		m_fScrollX -= 1.f;
	}
	if (m_bScrollShake)
	{
		if (m_dwShakeTime + m_dwShakeSpeed <= GetTickCount())
		{
			m_dwShakeSpeed -= 10.f;
			m_fScrollX -= 5 * m_Shake;

			m_Shake = m_Shake * -1;

			if (m_dwShakeSpeed < 10.f)
			{
				m_bScrollShake = false;
				STOP_PLAY_SOUND(L"EarthQuake.wav", CHANNELID::SOUND_EFFECT, 1.0f);
				ScrollUnLock();
				return;
			}
			m_dwShakeTime = GetTickCount();
		}
	}
	if (m_bScrollIntenseShake)
	{
		if (m_dwIntenseShakeTime + m_dwIntenseShakeSpeed <= GetTickCount())
		{
			m_dwIntenseShakeSpeed -= 1.0f;
			m_fScrollX -= 10 * m_Shake;

			m_Shake = m_Shake * -1;
			STOP_PLAY_SOUND(L"EarthQuake.wav", CHANNELID::SOUND_EFFECT, 0.6f);
			if (m_dwIntenseShakeSpeed < 10.f)
			{
				m_bScrollIntenseShake = false;
				m_bIntenseShakeFinished = true;
				ScrollUnLock();
				return;
			}
			m_dwIntenseShakeTime = GetTickCount();
		}
	}
}

void ScrollManager::ScrollShake()
{
	m_bScrollLock = true;
	m_dwShakeTime = GetTickCount();
	m_bScrollShake = true;

}

void ScrollManager::ScrollIntenseShake()
{
	m_bScrollLock = true;
	m_dwIntenseShakeTime = GetTickCount();
	m_bScrollIntenseShake = true;
}
