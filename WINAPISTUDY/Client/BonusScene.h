#pragma once
#include "Scene.h"

enum ROTATEDIRECTION
{
	RD_LEFT,
	RD_RIGHT,
	RD_END
};

class BonusScene : public Scene
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render();
	virtual void Release();


private:
	void Forward();
	void ForwardRender();
	void SonicWalkRender();
	void Rotate();
	void RotateRender();
	void BackgroundRender();

private:
	void MoveSonicWalkFrame();
	void MoveForwardFrame();
	void MoveRotateFrame();

	void Key_Input();

private:
	bool m_bRotating;
	
	FRAME m_ForwardFrame;
	FRAME m_RotateFrame;
	FRAME m_SonicWalkFrame;

	wstring m_MyFrameKey;
	wstring m_OtherFrameKey;
	wstring m_CurFrameKey;

	const TCHAR* m_CurSonicWalkFrameKey;
	bool m_bSonicWalkReverse;
	int m_SonicWalkImageLeftArr[3];
	int m_SonicWalkImageRightArr[3];
	ROTATEDIRECTION m_eRotateDirection;

	//RECT m_
	
	virtual void SelectFrameKey() {}
};

