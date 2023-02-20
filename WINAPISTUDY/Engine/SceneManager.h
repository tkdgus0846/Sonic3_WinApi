#pragma once
#include "Scene.h"

enum SELECTIMGOBJMODE
{
	SIOM_IMAGE,
	SIOM_OBSTACLE,
	SIOM_END
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager)

public:
	void Init(HWND hWnd);

	void Release();
	void Update();
	void Late_Update();
	void Render();

	void AddScene(string name, Scene* newScene);
	void SelectScene(string name);
	Scene* GetCurrentScene() { return m_CurScene; }
	void ActClear() { if (m_CurScene) m_CurScene->ActClear(); }

	void	Save_File(const TCHAR* pFilePath);
	void	Load_File(const TCHAR* pFilePath);

	void		SetSelectMode(EDITERSELECTMODE mode1, SELECTIMGOBJMODE mode2)
	{
		m_eSelectMode = mode1;
		m_eSIOMode = mode2;
	}

	void	MakeImageObject(POINT& pt, const TCHAR* frameKey, int zOrder, int coordX, int coordY, const Vec2& RealSize, bool collisionOn, int SplitMode = 0, bool freeTouch = false, bool canOverlap = false);
	void	EraseImageObject(POINT& pt);
	void	Mouse_Input(const TCHAR* frameKey, int zOrder, int coordX, int coordY, const Vec2& RealSize, bool collisionOn, int SplitMode = 0, bool freeTouch = false, bool canOverlap = false);

private:
	void RemoveScene(Scene* prevScene);
	void RemoveCurSceneObjList();
	void PassSceneObjects(Scene* prevScene, Scene* nextScene);
	void AddPassList(OBJID objID);
	void CalculateGrid(float& x, float& y, float fSizeX, float fSizeY, POINT& pt);
	Vec2 GetRenderSize(const TCHAR* frameKey, bool SplitMode = 0);
	
	class ImageObject* CreateImageObject(const TCHAR* frameKey, const Vec2& pos, const Vec2& size, const Vec2& RenderSize, int zOrder, int coordX, int coordY, bool collisionOn, int splitMode = 0);
	void DebugSaveAndLoad(int dwByte, string str)
	{
		if (dwByte == 0)
			cout << "dwByte: 0 " << str << endl;
	}

private:
	Scene*						m_CurScene;
	map<string, Scene*>			m_SceneList;
	list<OBJID>					m_PassList;

	HWND						m_Hwnd;
	EDITERSELECTMODE			m_eSelectMode;
	SELECTIMGOBJMODE			m_eSIOMode;
};

#define SCENE SceneManager::GetInstance()

