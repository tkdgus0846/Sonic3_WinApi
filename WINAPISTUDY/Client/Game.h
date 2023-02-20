#pragma once

class Game
{
public:
	Game();
	~Game();

	LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Init(HINSTANCE hInstance, int CmdShow);
	void Run();
	void Release();

private:
	void Update();
	void Late_Update();
	void Input();
	void Render();

private:
	HINSTANCE	m_hInst;
	LPCWSTR		m_szTitle;
	LPCWSTR		m_szWindowClass;
	RECT		m_windowRect;
	HWND		m_hWnd;

	int			m_iFPS;
	LONG		m_dwTime;
	TCHAR	    m_szFPS[32];
};

