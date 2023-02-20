#include "pch.h"
#include "LogoScene.h"
#include <LineManager.h>
#include <TileManager.h>
#include <SoundManager.h>

void LogoScene::Init()
{
	STOP_ALL_SOUND;
	m_logoFrame.iFrameStart = 0;
	m_logoFrame.iFrameEnd = 2;
	m_logoFrame.iMotion = 0;
	m_logoFrame.dwTime = GetTickCount();
	m_logoFrame.dwSpeed = 2000;
	m_logoFrame.repeat = false;

	m_AirplaneFrame.iFrameStart = 0;
	m_AirplaneFrame.iFrameEnd = 3;
	m_AirplaneFrame.iMotion = 0;
	m_AirplaneFrame.dwTime = GetTickCount();
	m_AirplaneFrame.dwSpeed = 150;
	m_AirplaneFrame.repeat = true;

	m_MainBackFrame.iFrameStart = 0;
	m_MainBackFrame.iFrameEnd = 2;
	m_MainBackFrame.iMotion = 0;
	m_MainBackFrame.dwTime = GetTickCount();
	m_MainBackFrame.dwSpeed = 200;
	m_MainBackFrame.repeat = true;

	m_MainSonicFrame.iFrameStart = 0;
	m_MainSonicFrame.iFrameEnd = 3;
	m_MainSonicFrame.iMotion = 0;
	m_MainSonicFrame.dwTime = GetTickCount();
	m_MainSonicFrame.dwSpeed = 120;
	m_MainSonicFrame.repeat = true;

	m_AirplanePosX = -100;
	m_TitlePosY = 330.f;
	m_bTitleUpDown = false;

	m_bSegaSoundPlayed = false;
	m_bTitleBGMPlayed = false;

	dwTime = GetTickCount();
}

void LogoScene::Update()
{
}

void LogoScene::Late_Update()
{
	Move_LogoFrame();
	Move_MainBackFrame();
	Move_MainSonicFrame();
	Move_AirplaneFrame();

	Key_Input();
}

void LogoScene::Render()
{
	
	if (dwTime + 1300 <= GetTickCount())
	{
		if (m_bTitleBGMPlayed == false)
		{
			PLAY_BGM(L"TitleScreen.wav", 1.0f);
			m_bTitleBGMPlayed = true;
		}
	}

	if (dwTime + 3500 <= GetTickCount())
	{
		
		
		RENDER->Animation_StretchBlt
		(
			L"MainBack",
			m_MainBackFrame,
			0, 0,
			800, 600,
			320, 240,
			0, 0
		);

		//52 24
		RENDER->Animation_TransparentBlt
		(
			L"Airplane",
			m_AirplaneFrame,
			++m_AirplanePosX, 90,
			104, 48,
			52, 24,
			RGB(255, 0, 255),
			0, 0
		);
	    
		// 240 170
		RENDER->Animation_TransparentBlt
		(
			L"MainSonic",
			m_MainSonicFrame,
			105, 30,
			240*2.5f, 170 * 2.5f,
			240, 170,
			RGB(255, 0, 255),
			0, 0
		);

		if (m_bTitleUpDown == false)
		{
			RENDER->TransparentImage
			(
				L"MainTitle",
				85, m_TitlePosY += 0.5f,
				260 * 2.4f, 80 * 2.4f,
				0, 0,
				260, 80,
				RGB(255, 0, 255)
			);
			if (m_TitlePosY >= 360) m_bTitleUpDown = true;
		}
		else
		{
			RENDER->TransparentImage
			(
				L"MainTitle",
				85, m_TitlePosY -= 0.5f,
				260 * 2.4f, 80 * 2.4f,
				0, 0,
				260, 80,
				RGB(255, 0, 255)
			);
			if (m_TitlePosY <= 340) m_bTitleUpDown = false;
		}
		

		RENDER->TransparentImage
		(
			L"MainYear",
			520,
			550,
			240,
			30,
			0,
			0,
			100,
			12,
			RGB(255, 0, 255)
		);

		if (INPUT->Key_Down(VK_RETURN))
		{
			
			SCENE->SelectScene("Scene01");
		}
	}
	else
	{
		// 320 240

		if (m_bSegaSoundPlayed == false)
		{
			PLAY_SOUND(L"SegaScreen.wav", SOUND_EFFECT, 1.f);
			m_bSegaSoundPlayed = true;
		}
		
		
		
		RENDER->Animation_StretchBlt
		(
			L"Logo",
			m_logoFrame,
			0,
			0,
			800,
			600,
			320,
			240,
			0,
			0
		);
	}
	
}

void LogoScene::Release()
{
}


void LogoScene::SelectFrameKey()
{
}

void LogoScene::Move_LogoFrame()
{
	if (m_logoFrame.dwTime + m_logoFrame.dwSpeed < GetTickCount())
	{
		if (m_logoFrame.iFrameStart == 2 && m_logoFrame.iMotion == 3)
		{

		}
		else
		{
			++m_logoFrame.iFrameStart;
		}
		m_logoFrame.dwSpeed = 120;

		if (m_logoFrame.iFrameStart > m_logoFrame.iFrameEnd)
		{
			m_logoFrame.iFrameStart = 0;

			if (m_logoFrame.iMotion + 1 < 4)
			{
				m_logoFrame.iMotion++;
			}
		}

		m_logoFrame.dwTime = GetTickCount();
	}
}

void LogoScene::Move_AirplaneFrame()
{
	if (m_AirplaneFrame.dwTime + m_AirplaneFrame.dwSpeed < GetTickCount())
	{
		++m_AirplaneFrame.iFrameStart;

		if (m_AirplaneFrame.iFrameStart > m_AirplaneFrame.iFrameEnd)
		{
			m_AirplaneFrame.iFrameStart = 0;
		}

		m_AirplaneFrame.dwTime = GetTickCount();
	}
}

void LogoScene::Move_MainBackFrame()
{
	if (m_MainBackFrame.dwTime + m_MainBackFrame.dwSpeed < GetTickCount())
	{
		++m_MainBackFrame.iFrameStart;

		if (m_MainBackFrame.iFrameStart > m_MainBackFrame.iFrameEnd)
		{
			m_MainBackFrame.iFrameStart = 0;
			++m_MainBackFrame.iMotion;
			if (m_MainBackFrame.iMotion == 2) 
				m_MainBackFrame.iMotion = 0;
		}

		m_MainBackFrame.dwTime = GetTickCount();
	}
}

void LogoScene::Move_MainSonicFrame()
{
	if (m_MainSonicFrame.dwTime + m_MainSonicFrame.dwSpeed < GetTickCount())
	{
		++m_MainSonicFrame.iFrameStart;

		if (m_MainSonicFrame.iFrameStart > m_MainSonicFrame.iFrameEnd)
		{
			m_MainSonicFrame.iFrameStart = 0;
			++m_MainSonicFrame.iMotion;
			if (m_MainSonicFrame.iMotion == 4)
				m_MainSonicFrame.iMotion = 0;
		}

		m_MainSonicFrame.dwTime = GetTickCount();
	}
}

void LogoScene::Key_Input()
{
	if (INPUT->Key_Up(VK_2))
	{
		SCENE->SelectScene("Scene02");
		return;
	}

	if (INPUT->Key_Up(VK_1))
	{
		SCENE->SelectScene("Scene01");
		return;

	}

	if (INPUT->Key_Up(VK_3))
	{
		SCENE->SelectScene("Scene03");
		return;

	}

}
