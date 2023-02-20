#pragma once


class InputManager
{
	DECLARE_SINGLE(InputManager)

public:
	void Init();
	void Update();
	void Release();

public:
	bool		Key_Pressing(int _iKey);
	bool		Key_Down(int _iKey);
	bool		Key_Up(int _iKey);

	bool		Key_NotPressing(int _iKey);

private:
	bool				m_bKeyState[VK_MAX];
};

#define INPUT InputManager::GetInstance()

