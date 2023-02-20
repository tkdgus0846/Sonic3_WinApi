#include "pch.h"
#include "InputManager.h"


void InputManager::Init()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

void InputManager::Update()
{
	if (Key_Down(VK_ESCAPE)) PostQuitMessage(0);
}

void InputManager::Release()
{
}

bool InputManager::Key_Pressing(int _iKey)
{
	if ((GetAsyncKeyState(_iKey) & 0x8000))
		return true;

	return false;
}

bool InputManager::Key_Down(int _iKey)
{
	// 이전에는 눌림이 없고, 현재 눌렸을 경우

	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = true;
		return true;
	}

	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
		m_bKeyState[_iKey] = false;

	return false;
}

bool InputManager::Key_Up(int _iKey)
{
	// 이전에 눌림은 있고, 현재 눌리지 않았을 경우

	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = false;
		return true;
	}

	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];

	return false;
}

bool InputManager::Key_NotPressing(int _iKey)
{
	return !m_bKeyState[_iKey];
}

