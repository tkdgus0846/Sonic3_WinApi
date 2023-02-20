#include "pch.h"
#include "SceneManager.h"
#include "ImageObject.h"
#include "ScrollManager.h"

#include "Spike.h"
#include "Roller.h"
#include "Ring.h"
#include "Spring.h"
#include "BGMushroom1.h"
#include "BGMushroom2.h"
#include "BGMushroom3.h"
#include "Mushroom.h"
#include "Item.h"
#include "Stone.h"
#include "Pipe.h"

void SceneManager::Init(HWND hWnd)
{
	m_Hwnd = hWnd;
	AddPassList(OBJ_PLAYER);
	m_eSIOMode = SIOM_END;
	srand((unsigned int)time(0));
}

void SceneManager::Release()
{
	for (auto it = m_SceneList.begin(); it != m_SceneList.end(); )
	{
		if (it->second)
		{
			it->second->Release();
			Safe_Delete(&(it->second));
		}
		it = m_SceneList.erase(it);	
	}
}

void SceneManager::Update()
{
	if (m_CurScene)
		m_CurScene->Update();
}

void SceneManager::Late_Update()
{
	if (m_CurScene)
		m_CurScene->Late_Update();
}

void SceneManager::Render()
{
	if (m_CurScene)
		m_CurScene->Render();
}

void SceneManager::AddScene(string name, Scene* newScene)
{
	m_SceneList.insert({ name,newScene });
}

void SceneManager::SelectScene(string name)
{
	Scene* prevScene = nullptr;
	if (m_CurScene)
		prevScene = m_CurScene;
		
	m_CurScene = m_SceneList[name];
	if (prevScene)
	{
		PassSceneObjects(prevScene, m_CurScene);
		RemoveScene(prevScene);
	}
	m_CurScene->Init();

	
}

void SceneManager::Save_File(const TCHAR* pFilePath)
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

	list<GameObject*>* imageList = m_CurScene->GetGameObjectslist(OBJ_IMAGE);
	list<GameObject*>* obsList = m_CurScene->GetGameObjectslist(OBJ_OBSTACLE);

	for (auto item : *imageList)
	{
		ImageObject* image = dynamic_cast<ImageObject*>(item);
		if (image == nullptr) continue;

		int zorder = image->GetZOrder();
		WriteFile(hFile, &zorder, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		TCHAR msg[32];
		lstrcpy(msg, image->GetFrameKeyArr());
		WriteFile(hFile, msg, sizeof(TCHAR) * 32, &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		int coordX = image->GetCoordX(); 
		WriteFile(hFile, &coordX, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		int coordY = image->GetCoordY(); 
		WriteFile(hFile, &coordY, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		Vec2 pos = image->GetPos();
		WriteFile(hFile, &pos, sizeof(Vec2), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		Vec2 size = image->GetSize();
		WriteFile(hFile, &size, sizeof(Vec2), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		bool collisionOn = image->GetCollisionOn();
		WriteFile(hFile, &collisionOn, sizeof(bool), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		int splitMode = image->GetSplitMode();
		WriteFile(hFile, &splitMode, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;
	}

	for (auto obs : *obsList)
	{
		ImageObject* image = dynamic_cast<ImageObject*>(obs);
		if (image == nullptr) continue;

		int zorder = image->GetZOrder();
		WriteFile(hFile, &zorder, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		TCHAR msg[32];
		lstrcpy(msg, image->GetFrameKeyArr());
		WriteFile(hFile, msg, sizeof(TCHAR) * 32, &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		int coordX = image->GetCoordX();
		WriteFile(hFile, &coordX, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		int coordY = image->GetCoordY();
		WriteFile(hFile, &coordY, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		Vec2 pos = image->GetPos();
		WriteFile(hFile, &pos, sizeof(Vec2), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		Vec2 size = image->GetSize();
		WriteFile(hFile, &size, sizeof(Vec2), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		bool collisionOn = image->GetCollisionOn();
		WriteFile(hFile, &collisionOn, sizeof(bool), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		int splitMode = image->GetSplitMode();
		WriteFile(hFile, &splitMode, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;
	}

	CloseHandle(hFile);

	MessageBox(m_Hwnd, _T("Save 성공"), L"성공", MB_OK);
}

void SceneManager::Load_File(const TCHAR* pFilePath)
{
	// 플레이어도 삭제되는 것을 생각하자. -> 나중에 처리
	RemoveCurSceneObjList();

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
	
	while (true)
	{
		
		int zorder;
		ReadFile(hFile, &zorder, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "ZOrder");
		if (0 == dwByte) break;

		TCHAR msg[32];
		ReadFile(hFile, msg, sizeof(TCHAR) * 32, &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "msg");
		if (0 == dwByte) break;

		int coordX;
		ReadFile(hFile, &coordX, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "coordX");
		if (0 == dwByte) break;

		int coordY;
		ReadFile(hFile, &coordY, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "coordY");
		if (0 == dwByte) break;

		Vec2 pos;
		ReadFile(hFile, &pos, sizeof(Vec2), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "pos");
		if (0 == dwByte) break;

		Vec2 size;
		ReadFile(hFile, &size, sizeof(Vec2), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "size");
		if (0 == dwByte) break;

		bool collisionOn;
		ReadFile(hFile, &collisionOn, sizeof(bool), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "collisionOn");
		if (0 == dwByte) break;

		int splitMode;
		ReadFile(hFile, &splitMode, sizeof(int), &dwByte, nullptr);
		DebugSaveAndLoad(dwByte, "splitMode");
		if (0 == dwByte) break;

		ImageObject* tmp = nullptr;
		Vec2 RenderSize = GetRenderSize(msg, splitMode);
		
		tmp = CreateImageObject(msg, pos, size, RenderSize, zorder, coordX, coordY, collisionOn, splitMode);
		
		if (collisionOn == true)
		{
			int i = 3;
		}
		

		if (tmp)
		{
			if (collisionOn == false)
				m_CurScene->AddGameObject(OBJ_IMAGE, tmp);
			else
				m_CurScene->AddGameObject(OBJ_OBSTACLE, tmp);
		}
			
	}

	// 파일 소멸
	CloseHandle(hFile);
}

void SceneManager::MakeImageObject(POINT& pt, const TCHAR* frameKey, int zOrder, int coordX, int coordY, const Vec2& RealSize, bool collisionOn, int SplitMode, bool freeTouch, bool canOverlap)
{
	bool bCollision = false;
	ImageObject* obj = nullptr;
	RECT tmp{};

	float x, y;

	Vec2 SizeVec = RealSize;

	if (freeTouch == false)
	{
		CalculateGrid(x, y, SizeVec.x, SizeVec.y, pt);
	}
	else
	{
		x = pt.x;
		y = pt.y;
	}
	
	// 클릭한 좌표를 격자 정보로 만들기 위한 계산

	Vec2 RenderSize = GetRenderSize(frameKey, SplitMode);

	obj = CreateImageObject(frameKey, { (float)x,(float)y }, SizeVec, RenderSize, zOrder, coordX, coordY, collisionOn, SplitMode);

	list<GameObject*>* imgList = m_CurScene->GetGameObjectslist(OBJ_IMAGE);
	list<GameObject*>* obsList = m_CurScene->GetGameObjectslist(OBJ_OBSTACLE);

	// 기존의 사각형과 겹치는지 검사.
	if (collisionOn == false)
	{
		for (auto iter : *imgList)
		{
			RECT blockRect = obj->GetArea();
			RECT otherRect = iter->GetArea();
			if (IntersectRect(&tmp, &blockRect, &otherRect))
			{
				bCollision = true;
				break;
			}
		}
	}
	else
	{
		for (auto iter : *obsList)
		{
			RECT blockRect = obj->GetArea();
			RECT otherRect = iter->GetArea();
			if (IntersectRect(&tmp, &blockRect, &otherRect))
			{
				bCollision = true;
				break;
			}
		}
	}
	

	if (canOverlap == false)
	{
		cout << "겹침 ." << endl;
		if (bCollision)
		{
			delete obj;
			return;
		}
	}
	// 겹치는 사각형이 있으면 생성한 정보를 지우기.
	

	if (collisionOn == false)
		m_CurScene->AddGameObject(OBJ_IMAGE, obj);
	else if (collisionOn == true)
		m_CurScene->AddGameObject(OBJ_OBSTACLE, obj);
}

void SceneManager::EraseImageObject(POINT& pt)
{
	float fSizeX, fSizeY;
	fSizeX = 64.f; // 그리드 X사이즈
	fSizeY = 64.f; // 그리드 Y사이즈

	RECT rect = { pt.x - fSizeX*0.5f,pt.y - fSizeY * 0.5f,pt.x + fSizeX * 0.5f,pt.y + fSizeY * 0.5f };
	RECT tmpRect;
	float tmpX;
	float tmpY;

	if (m_eSIOMode == SIOM_IMAGE)
	{
		list<GameObject*>* imgList = m_CurScene->GetGameObjectslist(OBJ_IMAGE);

		for (auto it = imgList->begin(); it != imgList->end(); it++)
		{
			RECT imgObjRect = (*it)->GetArea();
			if (IntersectRect(&tmpRect, &rect, &imgObjRect))
			{
				Safe_Delete(&*it);
				imgList->erase(it);
				return;
			}
		}
	}
	else if (m_eSIOMode == SIOM_OBSTACLE)
	{
		list<GameObject*>* imgList = m_CurScene->GetGameObjectslist(OBJ_OBSTACLE);

		for (auto it = imgList->begin(); it != imgList->end(); it++)
		{
			RECT imgObjRect = (*it)->GetArea();
			if (IntersectRect(&tmpRect, &rect, &imgObjRect))
			{
				Safe_Delete(&*it);
				imgList->erase(it);
				return;
			}
		}
	}

	

	
	
}

void SceneManager::RemoveScene(Scene* prevScene)
{
	for (auto it = m_SceneList.begin(); it != m_SceneList.end(); it++)
	{
		if (it->second == prevScene)
		{
			// 넘기지 않을 것들만 지워줌.
			for (int i = 0; i < OBJ_END; i++)
			{
				bool state = false;
				
				for (OBJID objId : m_PassList)
				{
					if (i == objId)
					{
						state = true;
						break;
					}
				}
				if (state == true) continue;

				it->second->RemoveGameObjectsList(i);
			}
			Safe_Delete(&it->second);
			m_SceneList.erase(it);
			break;
		}
	}
}

void SceneManager::RemoveCurSceneObjList()
{
	
	for (int i = 0; i < OBJ_END; i++)
	{
		bool state = false;
		for (OBJID pass : m_PassList)
		{
			if (i == pass)
			{
				state = true;
				break;
			}
		}
		if (state == true) continue;
		m_CurScene->RemoveGameObjectsList(i);
	}
	
}

void SceneManager::PassSceneObjects(Scene* prevScene, Scene* nextScene)
{
	for (OBJID id : m_PassList)
	{
		list<GameObject*>* objList = prevScene->GetGameObjectslist(id);
		if (objList->empty()) continue;
		for (auto item : *objList)
		{
			nextScene->AddGameObject(id, item);
		}
	}
}

void SceneManager::AddPassList(OBJID objID)
{
	auto iter = m_PassList.begin();
	for (; iter != m_PassList.end(); iter++)
	{
		if (*iter == objID) break;
	}
	if (iter != m_PassList.end()) return;
	m_PassList.push_back(objID);
}

void SceneManager::CalculateGrid(float& x, float& y, float fSizeX, float fSizeY, POINT& pt)
{
	x = (pt.x / 64.f);
	y = (pt.y / 64.f);
	if (pt.x < 0) x -= 1;
	if (pt.y < 0) y -= 1;

	x = round(x);
	y = round(y);

	x *= 64.f;
	y *= 64.f;
	x += (fSizeX * 0.5f);
	y += (fSizeY * 0.5f);
}

Vec2 SceneManager::GetRenderSize(const TCHAR* frameKey, bool SplitMode)
{
	Vec2 RenderSize;

	if (lstrcmp(frameKey, L"Spike") == 0)
	{
		RenderSize = { 31,31 };
	}
	else if (lstrcmp(frameKey, L"Roller") == 0)
	{
		RenderSize = { 66,13 };
	}
	else if (lstrcmp(frameKey, L"Ring2") == 0)
	{
		RenderSize = { 16,16 };
	}
	else if (lstrcmp(frameKey, L"DiagonalSpring") == 0)
	{
		RenderSize = { 36,36 };
	}
	else if (lstrcmp(frameKey, L"Spring") == 0)
	{
		RenderSize = { 32,32 };
	}
	else if (lstrcmp(frameKey, L"BGMushroom1") == 0)
	{
		RenderSize = { 72,40 };
	}
	else if (lstrcmp(frameKey, L"BGMushroom2") == 0)
	{
		RenderSize = { 16,27 };
	}
	else if (lstrcmp(frameKey, L"BGMushroom3") == 0)
	{
		RenderSize = { 16,32 };
	}
	else if (lstrcmp(frameKey, L"Mushroom") == 0)
	{
		RenderSize = { 70,60 };
	}
	else if (lstrcmp(frameKey, L"Item") == 0)
	{
		RenderSize = { 28,32 };
	}
	else if (lstrcmp(frameKey, L"Stone1") == 0)
	{
		RenderSize = { 32,29 };
	}
	else if (lstrcmp(frameKey, L"Cloud1") == 0)
	{
		RenderSize = { 32,24 };
	}
	else if (lstrcmp(frameKey, L"Cloud2") == 0)
	{
		RenderSize = { 64,24 };
	}
	else if (lstrcmp(frameKey, L"CloudTile") == 0)
	{
		RenderSize = { 16,16 };
	}
	else if (lstrcmp(frameKey, L"Pipe") == 0)
	{
		RenderSize = { 32,32 };
	}
	else
	{
		if (SplitMode == 0)
			RenderSize = { 128,128 };
		else
			RenderSize = { 64,64 };
	}
	return RenderSize;
}

ImageObject* SceneManager::CreateImageObject(const TCHAR* frameKey, const Vec2& pos, const Vec2& size, const Vec2& RenderSize, int zOrder, int coordX, int coordY, bool collisionOn, int splitMode)
{
	ImageObject* tmp;
	if (lstrcmp(frameKey, L"Spike") == 0)
	{
		tmp = new Spike(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Roller") == 0)
	{
		tmp = new Roller(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Spring") == 0)
	{
		tmp = new Spring(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, 0);
	}
	else if (lstrcmp(frameKey, L"DiagonalSpring") == 0)
	{
		tmp = new Spring(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, 1);
	}
	else if (lstrcmp(frameKey, L"Sign") == 0)
	{
		tmp = new ImageObject(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Ring2") == 0)
	{
		tmp = new Ring(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"BGMushroom1") == 0)
	{
		tmp = new BGMushroom1(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"BGMushroom2") == 0)
	{
		tmp = new BGMushroom2(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"BGMushroom3") == 0)
	{
		tmp = new BGMushroom3(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"DropMushroom") == 0)
	{
		tmp = new ImageObject(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Mushroom") == 0)
	{
		tmp = new Mushroom(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Item") == 0)
	{
		tmp = new Item(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Stone1") == 0)
	{
		tmp = new Stone(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Cloud1") == 0)
	{
		tmp = new ImageObject(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Cloud2") == 0)
	{
		tmp = new ImageObject(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"CloudTile") == 0)
	{
		tmp = new ImageObject(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else if (lstrcmp(frameKey, L"Pipe") == 0)
	{
		tmp = new Pipe(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}
	else
	{
		tmp = new ImageObject(zOrder, frameKey, coordX, coordY, pos, size, RenderSize, collisionOn, splitMode);
	}

	return tmp;
}

void SceneManager::Mouse_Input(const TCHAR* frameKey, int zOrder, int coordX, int coordY, const Vec2& RealSize, bool collisionOn, int SplitMode, bool freeTouch, bool canOverlap)
{
	if (INPUT->Key_Up(VK_LBUTTON))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_Hwnd, &pt);

		pt.x -= (int)SCROLLX;
		pt.y -= (int)SCROLLY;

		switch (m_eSelectMode)
		{
		case DRAW:
			MakeImageObject(pt, frameKey, zOrder, coordX, coordY, RealSize, collisionOn, SplitMode, freeTouch, canOverlap);
			break;
		case ERASE:
			EraseImageObject(pt);
			break;
		}
		
	}
	
}
