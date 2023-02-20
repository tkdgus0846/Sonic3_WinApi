#include "pch.h"
#include "Scene02.h"
#include "ScrollManager.h"
#include <SoundManager.h>
#include <LineManager.h>
#include <TileManager.h>
#include "Player.h"
#include <SpikeBall.h>
#include "Sign.h"

void Scene02::Init()
{
	Scene::Init();

	cout << "Scene02" << endl;
	SCROLL->ScrollClear();
	

	//AddGameObject(OBJ_PLAYER, FACTORY_CREATE(Player, Vec2( 79 * 64,-205 * 64 )));
	Player* player = nullptr;
	if (m_GameObjectList[OBJ_PLAYER].empty() == false)
		player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
	
	if (player)
	{
		Player* newPlayer = new Player(Vec2(380, 400), player->GetLife(), player->GetRings(), player->GetScore());

		newPlayer->SetPos(Vec2(380, 400));
		newPlayer->SetInvincible(true);
		auto iter = m_GameObjectList[OBJ_PLAYER].begin();

		Safe_Delete(&*iter);
		m_GameObjectList[OBJ_PLAYER].erase(iter);

		AddGameObject(OBJ_PLAYER, newPlayer);
		//player->SetDir({ 0,1 });
		
		
		//player->SetCurState(PS_DAMAGED);
	}
	else
	{
		AddGameObject(OBJ_PLAYER, FACTORY_CREATE(Player, Vec2(380, 400)));
		Player* newPlayer = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front()); 
		newPlayer->SetInvincible(true);
	}


	STOP_ALL_SOUND;
	SOUND->PlayBGM(L"LavaReefZone2.wav", 1.0f);

	LINE_MANAGER->Load_File(L"../Data/Line2.dat");
	TILE->Load_File(L"../Data/Tile2.dat");
	SCENE->Load_File(L"../Data/ImageObject2.dat");

	m_bSpikeBallSpawned = false;
	m_SpikeBall = nullptr;

	m_fEndPointX = 616*64.f;
	m_fEndPointY = 286*64.f;

	m_bSign = false;
	
}

void Scene02::Update()
{
	Scene::Update();
	LINE_MANAGER->Update();
	TILE->Update();



	if (m_SpikeBall && m_GameObjectList[OBJ_PLAYER].empty() == false)
	{
		Player* player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
		float followSpeed = player->GetSpeed();
		float AddSpeed = m_SpikeBall->GetAddSpeed();
		
		m_SpikeBall->SetSpeed(followSpeed + AddSpeed);
		

		LONG SpikeTime = 100 - followSpeed;
		LONG SoundTime = 500 - followSpeed * 2.5;
		if (SpikeTime < 10) SpikeTime = 10;

		m_SpikeBall->SetSpikeBallSpeedTime(SpikeTime);
		m_SpikeBall->SetSpikeBallSoundSpeed(SoundTime);


		if (player->GetPos().x < 91 * 64)
		{
			m_bSpikeBallSpawned = false;
			m_SpikeBall->SetDead();
		}

		if (m_SpikeBall->GetPos().x > 589 * 64)
		{
			m_SpikeBall->SetDead();
		}

		
		/*if (m_GameObjectList[OBJ_PLAYER].front()->GetPos().x > -573 * 64 && m_GameObjectList[OBJ_PLAYER].front()->GetPos().x < -570 * 64)
		{
			SCENE->SelectScene("Scene03");
			return;
		}*/

	}

	if (m_bSpikeBallSpawned == true) return;

	if (m_GameObjectList[OBJ_PLAYER].empty() == false)
	{
		Player* player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
		if (player)
		{
			if (m_bSpikeBallSpawned == false && player->GetPos().x > 91*64)
			{
				m_bSpikeBallSpawned = true;
				
				m_SpikeBall = new SpikeBall(2, L"SpikeBall", 0, 0, { 91 * 64,-207 * 64 }, { 64 * 2,64 * 2 }, { 64,64 }, true, 0);
				AddGameObject(OBJ_OBSTACLE, m_SpikeBall);
				
				
			}
		}
		
	}

	
	
}

void Scene02::Late_Update()
{
	Scene::Late_Update();
	COLLISION->Late_Update();
	COLLISION->Collision_Player_Line();
	COLLISION->Collision_Object_Line();
	COLLISION->Collision_With_Tile();
	COLLISION->Collision_With_Obstacle("PlayerBodyCollider",GetGameObjectslist(OBJ_OBSTACLE));
	COLLISION->Collision_With_ObstacleCollider("PlayerBodyCollider", GetGameObjectslist(OBJ_OBSTACLE));

	//if (m_bSign == false)
	//{
	//	GameObject* player = m_GameObjectList[OBJ_PLAYER].front();
	//	if (player)
	//	{
	//		int x = player->GetPos().x;
	//		//cout << x << " " << m_fEndPointX << endl;
	//		if (x >= m_fEndPointX)
	//		{
	//			//cout << "Appeard!" << endl;
	//			AddGameObject(OBJ_OBSTACLE, new Sign(2, L"Sign", 0, 0, { m_fEndPointX - 100,m_fEndPointY }, { 48 * 2,47 * 2 }, { 48,47 }, true));
	//			SCROLL_LOCK;
	//			m_bSign = true;
	//		}

	//	}
	//}

	Move_StartFrame();
}

void Scene02::Render()
{
	int      ScrollX = (int)SCROLLX;
	int      ScrollY = (int)SCROLLY;

	RENDER->SetPen(RGB(0x5c, 0x94, 0xfc));
	RENDER->SetBrush(RGB(0x5c, 0x94, 0xfc));
	RENDER->DrawRect(0, 0, 800, 600);
	// 1024 768
	RENDER->StretchImage_NoScroll
	(
		L"LavareepBack",
		0, 0,
		800, 600,
		0 - ScrollX * 0.1f, 300 - ScrollY * 0.01f,
		302, 236
	);
	RENDER->StretchImage_NoScroll
	(
		L"LavareepBack",
		0, 0,
		800, 600,
		-900 - ScrollX * 0.1f, 300 - ScrollY * 0.01f,
		302, 236
	);
	RENDER->StretchImage_NoScroll
	(
		L"LavareepBack",
		0, 0,
		800, 600,
		-1500 - ScrollX * 0.1f, 300 - ScrollY * 0.01f,
		302, 236
	);
	RENDER->StretchImage_NoScroll
	(
		L"LavareepBack",
		0, 0,
		800, 600,
		-2400 - ScrollX * 0.1f, 300 - ScrollY * 0.01f,
		302, 236
	);
	RENDER->StretchImage_NoScroll
	(
		L"LavareepBack",
		0, 0,
		800, 600,
		-3200 - ScrollX * 0.1f, 300 - ScrollY * 0.01f,
		302, 236
	);
	RENDER->StretchImage_NoScroll
	(
		L"LavareepBack",
		0, 0,
		800, 600,
		-4000 - ScrollX * 0.1f, 300 - ScrollY * 0.01f,
		302, 236
	);
	Scene::Render();

	if (m_bActClear == true)
	{
		RenderStageEnd(2);
	}
	else
	{
		if (m_bStartFrameEnded)
		{
			RenderScoreRings();
			RenderSonicLife();
		}
	}
	

	if (m_dwStartTime + 1500 >= GetTickCount())
	{
		RenderStageStart(2);
	}
	else
	{
		m_bStartFrameEnded = true;
	}

	COLLISION->Render();
}

void Scene02::Release()
{
	Scene::Release();
	LINE_MANAGER->Release();
	TILE->Release();
}


void Scene02::SelectFrameKey()
{
}
