#pragma once

#include "Tile.h"

class TileManager
{
	DECLARE_SINGLE(TileManager)

	
public:
	void AddTile(const Vec2& pos, const Vec2& size);

	void MakeTile(POINT& pt1, POINT& pt2);
	void EraseTile(POINT& pt);

	void Init(HWND hwnd);
	void Render();
	void Update();
	void Release();

	void		CalculateGrid(int& x, int& y, float fSizeX, float fSizeY, POINT& pt);

	void		Save_File(const TCHAR* pFilePath);
	void		Load_File(const TCHAR* pFilePath);

	void		Mouse_Input();
	void		Key_Input();

	void		SetSelectMode(EDITERSELECTMODE mode)
	{
		m_eSelectMode = mode;
	}

	list<Tile*>* GetTileList()
	{
		return &m_TileList;
	}


private:
	list<Tile*> m_TileList;
	HWND m_Hwnd;

	POINT pt1;
	POINT pt2;

	EDITERSELECTMODE m_eSelectMode;

};

#define TILE TileManager::GetInstance()

