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
	

private:
	DWORD dwTime;
	FRAME m_logoFrame;

	// Scene��(��) ���� ��ӵ�
	virtual void SelectFrameKey() override;

};

