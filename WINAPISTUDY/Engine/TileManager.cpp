#include "pch.h"
#include "TileManager.h"
#include "ScrollManager.h"

#define LONG_MIN -2147483647

void TileManager::AddTile(const Vec2& pos, const Vec2& size)
{
	Tile* newTile = new Tile(pos, size);
	m_TileList.push_back(newTile);
}



void TileManager::Init(HWND hwnd)
{
	m_Hwnd = hwnd;
	pt1 = { LONG_MIN,LONG_MIN };
	pt2 = { LONG_MIN,LONG_MIN };

	m_eSelectMode = DRAW;
}

void TileManager::Render()
{
	for (Tile* tile : m_TileList)
	{
		tile->Render();
	}
}

void TileManager::Update()
{
	for (Tile* tile : m_TileList)
	{
		tile->Update();
	}
}

void TileManager::Release()
{
	for (auto it = m_TileList.begin(); it != m_TileList.end(); it++)
	{
		if (*it)
		{
			Safe_Delete(&*it);
		}
	}
	m_TileList.clear();
}

void TileManager::CalculateGrid(int& x, int& y, float fSizeX, float fSizeY, POINT& pt)
{
	x = (pt.x / fSizeX);
	y = (pt.y / fSizeY);
	if (pt.x < 0) x -= 1;
	if (pt.y < 0) y -= 1;

	x *= fSizeX;
	y *= fSizeY;
	x += (fSizeX * 0.5f);
	y += (fSizeY * 0.5f);
}

void TileManager::Save_File(const TCHAR* pFilePath)
{
	// 파일 개방
	HANDLE		hFile = CreateFile(
		pFilePath,
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

	for (auto iter : m_TileList)
	{
		WriteFile(hFile, (iter), sizeof(Tile), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(m_Hwnd, _T("Save 성공"), L"성공", MB_OK);
}

void TileManager::Load_File(const TCHAR* pFilePath)
{
	Release();

	// 파일 개방
	HANDLE		hFile = CreateFile(
		pFilePath,
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
	Tile		tInfo;

	while (true)
	{
		// 파일 쓰기
		ReadFile(hFile, &tInfo, sizeof(Tile), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		Tile* tmp = nullptr;
		tmp = new Tile(tInfo.GetPos(), tInfo.GetSize());

		if (tmp)
			m_TileList.push_back(tmp);
	}

	// 파일 소멸
	CloseHandle(hFile);

	//MessageBox(m_Hwnd, _T("Load 성공"), L"성공", MB_OK);
}

void TileManager::Mouse_Input()
{
	if (INPUT->Key_Up(VK_LBUTTON))
	{
		
		GetCursorPos(&pt2);
		ScreenToClient(m_Hwnd, &pt2);
		
		pt2.x -= (int)SCROLLX;
		pt2.y -= (int)SCROLLY;
		cout << pt2.x << " " << pt2.y << endl;

		switch (m_eSelectMode)
		{
		case DRAW:
			MakeTile(pt1, pt2);
			break;
		case ERASE:
			EraseTile(pt2);
			break;
		}
		
		pt1.x = LONG_MIN;
		pt1.y = LONG_MIN;
	}
	if (INPUT->Key_Pressing(VK_LBUTTON))
	{
		if (pt1.x == LONG_MIN && pt1.y == LONG_MIN)
		{
			GetCursorPos(&pt1);
			ScreenToClient(m_Hwnd, &pt1);
			pt1.x -= (int)SCROLLX;
			pt1.y -= (int)SCROLLY;
		}
	}
	
}

void TileManager::Key_Input()
{
	
}

void TileManager::MakeTile(POINT& pt1, POINT& pt2)
{
	bool bCollision = false;
	Tile* pTile = nullptr;
	RECT tmp{};

	int x1, y1;
	int x2, y2;

	float fSizeX, fSizeY;
	fSizeX = 64.f; // 그리드 X사이즈
	fSizeY = 64.f; // 그리드 Y사이즈

	CalculateGrid(x1, y1, fSizeX, fSizeY, pt1);
	CalculateGrid(x2, y2, fSizeX, fSizeY, pt2);
	// 클릭한 좌표를 격자 정보로 만들기 위한 계산

	int left = ((x1 < x2) ? x1 : x2) - (fSizeX * 0.5f);
	int top = ((y1 < y2) ? y1 : y2) - (fSizeY * 0.5f);
	int right = ((x1 < x2) ? x2 : x1) + (fSizeX * 0.5f);
	int bottom = ((y1 < y2) ? y2 : y1) + (fSizeY * 0.5f);

	int cx = abs(x2 - x1) + fSizeX;
	int cy = abs(y2 - y1) + fSizeY;

	RECT rctTmp = { left,top,right,bottom };
	Vec2 sizeTmp = { (float)cx, (float)cy };
	pTile = new Tile(rctTmp, sizeTmp);

	// 기존의 사각형과 겹치는지 검사.
	for (auto& iter : m_TileList)
	{
		RECT blockRect = pTile->GetRect();
		RECT otherRect = iter->GetRect();
		if (IntersectRect(&tmp, &blockRect, &otherRect))
		{
			bCollision = true;
			break;
		}
	}

	// 겹치는 사각형이 있으면 생성한 정보를 지우기.
	if (bCollision)
	{
		delete pTile;
		return;
	}

	m_TileList.push_back(pTile);
}

void TileManager::EraseTile(POINT& pt)
{
	for (auto it = m_TileList.begin(); it != m_TileList.end(); it++)
	{
		RECT rect = (*it)->GetArea();
		if (rect.left <= pt.x && rect.right >= pt.x && rect.top <= pt.y && rect.bottom >= pt.y)
		{
			Safe_Delete(&*it);
			m_TileList.erase(it);
			return;
		}
	}
}
