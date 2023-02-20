#include "pch.h"
#include "LogoScene.h"

void LogoScene::Init()
{
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 8;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 175;
	m_Frame.repeat = true;

	m_logoFrame.iFrameStart = 0;
	m_logoFrame.iFrameEnd = 2;
	m_logoFrame.iMotion = 0;
	m_logoFrame.dwTime = GetTickCount();
	m_logoFrame.dwSpeed = 2000000;
	m_logoFrame.repeat = false;

	dwTime = GetTickCount();
}

void LogoScene::Update()
{
}

void LogoScene::Late_Update()
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
	Move_Frame();
}

void LogoScene::Render()
{
	

	if (dwTime + 3700 <= GetTickCount())
	{
		
		RENDER->Animation_StretchBlt
		(
			L"MainMenu2",
			m_Frame,
			0,
			0,
			800,
			600,
			320,
			224,
			0,
			0
		);

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
