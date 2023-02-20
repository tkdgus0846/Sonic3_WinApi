#pragma once

#include "Line.h"



// 선을 좌우로만 나누지말고 상하도 나눠보면 어떨까?

class LineManager
{
	DECLARE_SINGLE(LineManager)

public:

	void AddLine(int x1, int y1, int x2, int y2);
	//bool Collision_Line(float fx, float* fy);

	void Init(HWND hwnd);
	void Render();
	void Update();
	void Release();

	void		Save_File(const TCHAR* pFilePath);
	void		Load_File(const TCHAR* pFilePath);

	void		EraseLine(POINT& pt);

	void		Mouse_Input();
	void		Key_Input();

	void		SetSelectMode(EDITERSELECTMODE mode, LINELAYER layer)
	{
		m_eSelectMode = mode;
		m_eLineLayer = layer;
	}

	vector<Line*>* GetLineVec()
	{
		return &m_LineVec;
	}

	int GetZLayer(int i);
	int GetZLayer(Line* curLine);


private:
	vector<Line*>			m_LineVec;
	
	HWND					m_Hwnd;
	Vec2					m_LinePoint[LINE_END];

	EDITERSELECTMODE		m_eSelectMode;
	LINELAYER				m_eLineLayer;

	int						m_TotalLineNum;
};

#define LINE_MANAGER LineManager::GetInstance()
#define ZLAYER(line) LINE_MANAGER->GetZLayer(line)

