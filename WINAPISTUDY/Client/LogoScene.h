#pragma once
#include "Scene.h"

class LogoScene : public Scene
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render();
	virtual void Release();


private:
	DWORD dwTime;
	FRAME m_logoFrame;
	FRAME m_AirplaneFrame;
	FRAME m_MainBackFrame;
	FRAME m_MainSonicFrame;

	// Scene을(를) 통해 상속됨
	virtual void SelectFrameKey() override;

private:
	void Move_LogoFrame();
	void Move_AirplaneFrame();
	void Move_MainBackFrame();
	void Move_MainSonicFrame();

	void Key_Input();

private:
	int m_AirplanePosX;
	float m_TitlePosY;
	bool m_bTitleUpDown;

	bool m_bSegaSoundPlayed;
	bool m_bTitleBGMPlayed;

};

