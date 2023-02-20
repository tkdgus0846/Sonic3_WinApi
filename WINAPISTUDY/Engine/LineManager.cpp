#include "pch.h"
#include "LineManager.h"
#include "ScrollManager.h"

#define INIT_FLOAT -21678.88f

void LineManager::AddLine(int x1, int y1, int x2, int y2)
{
	/*Line* newLine = nullptr;

	Vec2 lp1 = Vec2(x1, y1);
	Vec2 lp2 = Vec2(x2, y2);
	newLine = new Line(lp1, lp2);

	m_LineVec.push_back(newLine);*/
}

void LineManager::Init(HWND hwnd)
{
	m_Hwnd = hwnd;

	m_LinePoint[LINE_LEFT].x = INIT_FLOAT;
	m_LinePoint[LINE_LEFT].y = INIT_FLOAT;
	
	m_eSelectMode = DRAW;

	m_TotalLineNum = 0;
}

void LineManager::Render()
{
	for (auto item : m_LineVec)
		item->Render();
		
}

void LineManager::Update()
{
	for (auto item : m_LineVec)
		item->Update();
}

void LineManager::Release()
{
	
	for (auto it = m_LineVec.begin(); it != m_LineVec.end();)
	{
		if (*it)
		{
			Safe_Delete(&*it);
		}
		it = m_LineVec.erase(it);
	}
	
	
}

void LineManager::Save_File(const TCHAR* pFilePath)
{
	// 파일 개방
	HANDLE		hFile = CreateFile
		(pFilePath,		
		GENERIC_WRITE,
		NULL,					
		NULL,					
		CREATE_ALWAYS,			
		FILE_ATTRIBUTE_NORMAL,  
		NULL);					

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(m_Hwnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD		dwByte = 0;

	WriteFile(hFile, &m_TotalLineNum, sizeof(int), &dwByte, nullptr);

	for (auto iter : m_LineVec)
	{
		WriteFile(hFile, iter, sizeof(Line), &dwByte, nullptr);
	}
	

	CloseHandle(hFile);

	MessageBox(m_Hwnd, _T("Save 성공"), L"성공", MB_OK);
}

void LineManager::Load_File(const TCHAR* pFilePath)
{
	Release();

	// 파일 개방
	HANDLE		hFile = CreateFile
		(pFilePath,
		GENERIC_READ,			
		NULL,					
		NULL,					
		OPEN_EXISTING,			
		FILE_ATTRIBUTE_NORMAL,  
		NULL);					

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(m_Hwnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD		dwByte = 0;
	Line line;

	ReadFile(hFile, &m_TotalLineNum, sizeof(int), &dwByte, nullptr);
	m_LineVec.reserve(m_TotalLineNum);

	while (true)
	{
		ReadFile(hFile, &line, sizeof(Line), &dwByte, nullptr);	

		if (0 == dwByte)
			break;

		Line* tmp = nullptr;
		tmp = new Line(line.GetPoint(LINE_LEFT), line.GetPoint(LINE_RIGHT), line.GetLayer(), line.GetLineNum());

		if (tmp)
			m_LineVec.push_back(tmp);
	}

	// 파일 소멸
	CloseHandle(hFile);

	//MessageBox(m_Hwnd, _T("Load 성공"), L"성공", MB_OK);
}

void LineManager::EraseLine(POINT& pt)
{
	float fSizeX, fSizeY;
	fSizeX = 16.f; // 그리드 X사이즈
	fSizeY = 16.f; // 그리드 Y사이즈

	RECT rect = { pt.x - fSizeX,pt.y - fSizeY,pt.x + fSizeX,pt.y + fSizeY };
	float tmpX;
	float tmpY;

	
	for (auto it = m_LineVec.begin(); it != m_LineVec.end(); it++)
	{
		if (COLLISION->IsRectInLine(&rect, *it, &tmpX, &tmpY))
		{
			Safe_Delete(&*it);
			m_LineVec.erase(it);
			m_TotalLineNum--;
			return;
		}
	}
	
	
}

void LineManager::Mouse_Input()
{
	POINT		pt{};

	GetCursorPos(&pt);
	ScreenToClient(m_Hwnd, &pt);

	pt.x -= (int)SCROLLX;
	pt.y -= (int)SCROLLY;

	if (INPUT->Key_Up(VK_LBUTTON))
	{
		m_LinePoint[LINE_RIGHT].x = (float)pt.x;
		m_LinePoint[LINE_RIGHT].y = (float)pt.y;

		if (m_eSelectMode == DRAW)
		{
			auto iter = m_LineVec.begin();
			int curNum = 0;
			for (; iter != m_LineVec.end(); iter++, curNum++)
			{
				if ((*iter)->GetLineNum() != curNum) break;
			}

			if (iter == m_LineVec.end())
			{
				m_LineVec.push_back(new Line(m_LinePoint[LINE_LEFT], m_LinePoint[LINE_RIGHT], m_eLineLayer, curNum));
				++m_TotalLineNum;
			}
			else
			{
				m_LineVec.insert(iter, new Line(m_LinePoint[LINE_LEFT], m_LinePoint[LINE_RIGHT], m_eLineLayer, curNum));
			}			
		}
			

		m_LinePoint[LINE_LEFT].x = INIT_FLOAT;
		m_LinePoint[LINE_LEFT].y = INIT_FLOAT;
	}

	if (INPUT->Key_Pressing(VK_LBUTTON))
	{
		if (m_LinePoint[LINE_LEFT].x == INIT_FLOAT
			&& m_LinePoint[LINE_LEFT].y == INIT_FLOAT)
		{
			if (m_eSelectMode == DRAW)
			{
				m_LinePoint[LINE_LEFT].x = (float)pt.x;
				m_LinePoint[LINE_LEFT].y = (float)pt.y;
			}
			else if (m_eSelectMode == ERASE)
			{
				EraseLine(pt);
			}
			
		}
		
	}
}

void LineManager::Key_Input()
{
}

int LineManager::GetZLayer(int i)
{
	if (i > 67)
	{
		return 3;
	}
	if (i > 55)
	{
		return 2;
	}
	if (i > 12)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int LineManager::GetZLayer(Line* curLine)
{
	return GetZLayer(curLine->GetLineNum());
}
