#pragma once

#include "Scene.h"

class EditScene : public Scene
{
	enum DRAWMODE
	{
		DM_TILE,
		DM_FLINE,
		DM_RLINE,
		DM_SPIKE,
		DM_ROLLER,
		DM_RING,
		DM_SPRING,
		DM_DIAGONALSPRING,
		DM_BGMUSH1,
		DM_BGMUSH2,
		DM_BGMUSH3,
		DM_MUSHROOM,
		DM_ITEM,
		DM_STONE,
		DM_BGBLACK,
		DM_CLOUD1,
		DM_CLOUD2,
		DM_CLOUDTILE,
		DM_PIPE,
		DM_NUM1,
		DM_NUM2,
		DM_NUM3,
		DM_NUM4,
		DM_NUM5,
		DM_NUM6,
		DM_NUM7,
		DM_NUM8,
		DM_NUM9,
		DM_DIVIDE,
		DM_PRODUCT,
		DM_MINUS,
		DM_END
	};

public:
	virtual void Init();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render();
	virtual void Release();

private:
	DWORD dwTime;
	DRAWMODE drawmode;
	EDITERSELECTMODE selectMode;
	const TCHAR* m_FrameKeyArr[2];


	int m_NowFrameKeyIndex;
	int m_NowCoordX;
	int m_NowCoordY;
	int m_NowZOrder;
	int m_SplitMode;
	float m_NowSize;
	bool bShowCoord;
	int m_NowSpikeNum;
	int m_NowItemNum;


	// Scene을(를) 통해 상속됨
	virtual void SelectFrameKey() override;
	void KeyInput();
	void MouseInput();

	void RenderCoord();
	void RenderHotKey();
};

