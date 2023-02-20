#include "pch.h"
#include "Scene03.h"
#include <ScrollManager.h>
#include <SoundManager.h>
#include "Player.h"
#include "Meteor.h"
#include "Rocket.h"
#include "DoomsDay.h"
#include "Ring.h"

void Scene03::Init()
{
	Scene::Init();
	SCROLL->ScrollClear();
	SCROLL->SetAutoScroll(true);
    STOP_ALL_SOUND;
	PLAY_BGM(L"DoomsDayZone.wav", 0.3f);

    Player* player = nullptr;
    if (m_GameObjectList[OBJ_PLAYER].empty() == false)
        player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());

    if (player)
    {
        Player* newPlayer = new Player(Vec2(300, 200), player->GetLife(), player->GetRings(), player->GetScore());

        newPlayer->SetPos(Vec2(300, 200));
        newPlayer->SetSuperMode();
        auto iter = m_GameObjectList[OBJ_PLAYER].begin();

        Safe_Delete(&*iter);
        m_GameObjectList[OBJ_PLAYER].erase(iter);

        AddGameObject(OBJ_PLAYER, newPlayer);
    }
    else
    {
        AddGameObject(OBJ_PLAYER, FACTORY_CREATE(Player, Vec2(300, 200)));
        Player* newPlayer = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
        newPlayer->SetSuperMode();
    }

   /* Player* player = new Player(Vec2(300, 200));
    player->Init();
    player->SetSuperMode();
    AddGameObject(OBJ_PLAYER, player);*/

    m_dwRandomMeteorTime = GetTickCount();
    m_dwSpawnRingsTime = GetTickCount();
    m_dwSpawnRocketTime = GetTickCount();
    m_dwPattern1Time = GetTickCount();
    m_dwPatternSelectTime = GetTickCount();

    m_bPattern1 = false;
    m_bPattern1State = false;
    m_bBossSpawned = false;
    m_iPattern1Y = 300;

    m_bRectangleDrawFinished = false;

    m_topY = 0;
    m_bottomY = 600;
    m_dwBossGenTime = GetTickCount();
    m_dwStageStartTime = GetTickCount();
    m_dwStageClearSceneTime = GetTickCount() + 10000000;
    m_bStageStart = false;

    m_bEndingCutScene = false;

    m_EndingSongVolume = 1.0f;
}

void Scene03::Update()
{
	Scene::Update();
}

void Scene03::Late_Update()
{
	Scene::Late_Update();
    COLLISION->Late_Update();
    COLLISION->Collision_With_Obstacle("PlayerShootingCollider", GetGameObjectslist(OBJ_OBSTACLE));
    COLLISION->Collision_With_ObstacleCollider("PlayerShootingCollider", GetGameObjectslist(OBJ_OBSTACLE));
    COLLISION->Collision_With_ObstacleCollider("DoomsDayCollider", GetGameObjectslist(OBJ_OBSTACLE));

    StageStart();
    RandomMeteorGenerate();
    SpawnRings();
    SpawnRocket();
    PatternSelect();
    SpawnBoss();
    Pattern1();
    FlyAwaySonicByeBye();
    EndingSongDownVolume();
}

void Scene03::Render()
{
    if (m_bEndingCutScene == false)
	    BackgroundRender();
	Scene::Render();

    COLLISION->Render();

    if (m_bEndingCutScene == true)
    {
        /*static int musicStop = 0;
        if (musicStop == 0)
        {
            STOP_SOUND(CHANNELID::SOUND_BGM);
        }
        musicStop++;*/
        RENDER->StretchImage_NoScroll
        (
            L"Ending",
            0, 0,
            800, 600,
            0, 0,
            320, 224
        );
        return;
    }
    
    if (m_bActClear == true)
    {
        DrawRectangleEndingScene();
        RENDER->SetBrush(RGB(0, 0, 0));
        RENDER->SetPen(RGB(0, 0, 0));
        RENDER->DrawRect(0, 0, 800, m_topY);
        RENDER->DrawRect(0, m_bottomY, 800, 600);
        if (m_bRectangleDrawFinished == true)
        {
            static int musicPlay = 0;
            if (musicPlay == 0)
            {
                m_dwStageClearSceneTime = GetTickCount();
                PLAY_BGM(L"EndingBGM.wav", 1.0f);
            }
            musicPlay++;
        }
    }
    if (m_bActClear == false)
    {
        RenderScoreRings();
        RenderSonicLife();
    }
    
}

void Scene03::Release()
{
	Scene::Release();
}

void Scene03::SelectFrameKey()
{
}

void Scene03::BackgroundRender()
{
    int      ScrollX = (int)SCROLLX;  
    int      ScrollY = (int)SCROLLY;
    // 1024 768

    RENDER->StretchImage_NoScroll
    (
        L"BGLayer1",
        0, 0,
        800, 176 * 1.5,
        0, 0,
        512, 176
    );
    int OffsetX = -512;
    for (int i = 0; i < 100; i++)
    {
        
        RENDER->StretchImage_NoScroll
        (
            L"BGLayer2",
            0, 264,
            800, 8 * 2,
            OffsetX * i - ScrollX * 0.1f, 0,
            512, 8
        );
        RENDER->StretchImage_NoScroll
        (
            L"BGLayer3",
            0, 280,
            800, 8 * 2,
            OffsetX * i - ScrollX * 0.5f, 0,
            512, 8
        );
        RENDER->StretchImage_NoScroll
        (
            L"BGLayer4",
            0, 296,
            800, 8 * 2,
            OffsetX * i - ScrollX * 0.9f, 0,
            512, 8
        );
        RENDER->StretchImage_NoScroll
        (
            L"BGLayer5",
            0, 312,
            800, 16 * 2,
            OffsetX * i - ScrollX * 1.3f, 0,
            512, 16
        );
        RENDER->StretchImage_NoScroll
        (
            L"BGLayer6",
            0, 344,
            800, 24 * 2,
            OffsetX * i - ScrollX * 1.7f, 0,
            512, 24
        );
        RENDER->StretchImage_NoScroll
        (
            L"BGLayer7",
            0, 392,
            800, 48 * 2,
            OffsetX * i - ScrollX * 2.1f, 0,
            512, 48
        );
        RENDER->StretchImage_NoScroll
        (
            L"BGLayer8",
            0, 488,
            800, 96 * 2,
            OffsetX * i - ScrollX * 2.5f, 0,
            512, 96
        );
    }
    
}

void Scene03::RandomMeteorGenerate()
{
    //if (m_bPattern1 == true) return;
    if (m_bStageStart == false) return;

    if (m_bBossSpawned == false)
    {
        if (m_dwRandomMeteorTime + 300 <= GetTickCount())
        {
            int      ScrollX = (int)SCROLLX;
            int      randY = rand() % 500 + 100;

            AddGameObject(OBJ_OBSTACLE, new Meteor(3, L"", 0, 0, { (float)(900 - ScrollX), (float)(randY) }, { 0,0 }, { 0,0 }, true, 0));

            m_dwRandomMeteorTime = GetTickCount();
        }
    }
    else
    {
        return;
        if (m_dwRandomMeteorTime + 2000 <= GetTickCount())
        {
            int      ScrollX = (int)SCROLLX;
            int      randY = rand() % 500 + 100;

            AddGameObject(OBJ_OBSTACLE, new Meteor(3, L"", 0, 0, { (float)(900 - ScrollX), (float)(randY) }, { 0,0 }, { 0,0 }, true, 0));

            m_dwRandomMeteorTime = GetTickCount();
        }
    }
    
    
}

void Scene03::SpawnBoss()
{
    // 원래 80000 이였음.
    if (m_dwBossGenTime + 80000 <= GetTickCount())
    {
        static bool i = false;
        if (i == false)
        {
            Creature* player = dynamic_cast<Creature*>(GetGameObjectslist(OBJ_PLAYER)->front());
            int      ScrollX = (int)SCROLLX;

            if (player)
            {
                m_pBoss = new DoomsDay({ (float)(1200 - ScrollX), (float)(-300) }, this, player);
                AddGameObject(OBJ_MONSTER,m_pBoss);
                m_bBossSpawned = true;
            }
            i = true;
        }
        m_dwBossGenTime = GetTickCount() + 700000;
    }
      
}

void Scene03::SpawnRings(int y)
{
    //if (m_bPattern1 == true) return;
    if (m_bBossSpawned == true) return;
    if (m_bStageStart == false) return;

    if (m_dwSpawnRingsTime + 4000 <= GetTickCount())
    {
        int      ScrollX = (int)SCROLLX;
        int      randY = rand() % 500 + 100;
        if (y == 0)
        {
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(900 - ScrollX) ,(float)(randY) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(950 - ScrollX) ,(float)(randY) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(1000 - ScrollX) ,(float)(randY) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(1050 - ScrollX) ,(float)(randY) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
        }
        else
        {
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(900 - ScrollX) ,(float)(y) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(950 - ScrollX) ,(float)(y) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(1000 - ScrollX) ,(float)(y) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
            AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(1050 - ScrollX) ,(float)(y) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
        }
        

        m_dwSpawnRingsTime = GetTickCount();
    }
    
}

void Scene03::SpawnRocket()
{
    if (m_bPattern1 == true) return;
    if (m_bBossSpawned == true) return;
    if (m_bStageStart == false) return;

    if (m_dwSpawnRocketTime + 2000 <= GetTickCount())
    {
        int      ScrollX = (int)SCROLLX;
        int      randY = rand() % 500 + 100;

        AddGameObject(OBJ_OBSTACLE, new Rocket(4, { (float)(900 - ScrollX), (float)(randY) }, { -1, 0 }, false, nullptr));
        m_dwSpawnRocketTime = GetTickCount();
    }
    
}

void Scene03::Pattern1()
{
    if (m_bPattern1 == false) return;
    if (m_bBossSpawned == true) return;
    if (m_bStageStart == false) return;

    int      ScrollX = (int)SCROLLX;

    if (m_dwPattern1Time + 1000 <= GetTickCount())
    {
        AddGameObject(OBJ_OBSTACLE, new Rocket(4, { (float)(900 - ScrollX), (float)(m_iPattern1Y) }, { -1, 0 }, false, nullptr));
        AddGameObject(OBJ_OBSTACLE, new Ring(4, L"Ring2", 0, 0, { (float)(940 - ScrollX) ,(float)(m_iPattern1Y + 100) }, { 32.f,32.f }, { 16.f,16.f }, true, 0, true));
        AddGameObject(OBJ_OBSTACLE, new Rocket(4, { (float)(900 - ScrollX), (float)(m_iPattern1Y + 200) }, { -1, 0 }, false, nullptr));

       

        if (m_iPattern1Y < 100.f)
            m_bPattern1State = true;

        if (m_bPattern1State == false)
            m_iPattern1Y -= 20.f;
        else
            m_iPattern1Y += 20.f;

        m_dwPattern1Time = GetTickCount();
        if (m_bPattern1State == true)
        {
            if (m_iPattern1Y > 400)
            {
                m_bPattern1State = false;
                m_bPattern1 = false;
                m_iPattern1Y = 300;
                m_dwPatternSelectTime = GetTickCount();
                return;
            }
        }
        
    }

    
}

void Scene03::Pattern2()
{
}

void Scene03::PatternSelect()
{
    if (m_bBossSpawned == true) return;
    if (m_bStageStart == false) return;

    if (m_dwPatternSelectTime + 30000 <= GetTickCount())
    {
        if (m_bPattern1 == false)
            m_bPattern1 = true;

        m_dwPatternSelectTime = GetTickCount();
    }
}

void Scene03::DrawRectangleEndingScene()
{
    if (m_bRectangleDrawFinished == true) return;

    if (m_dwDrawRectangleTime + 20 <= GetTickCount())
    {
        

        m_topY += 1;
        m_bottomY -= 1;

        if (m_topY > 100) m_topY = 100;
        if (m_bottomY < 500) m_bottomY = 500;

        if (m_topY == 100 && m_bottomY == 500) m_bRectangleDrawFinished = true;
        m_dwDrawRectangleTime = GetTickCount();
    }
    
}

void Scene03::StageStart()
{
    if (m_bStageStart == true) return;
    if (m_dwStageStartTime + 5000 <= GetTickCount())
    {
        m_bStageStart = true;
        return;
    }
}

void Scene03::FlyAwaySonicByeBye()
{
    if (m_bEndingCutScene == true) return;
    if (m_bActClear == false) return;
    if (m_dwStageClearSceneTime + 13000 <= GetTickCount())
    {
        static int i = 0;

        if (i == 1)
        {
            Player* player = dynamic_cast<Player*>(GetGameObjectslist(OBJ_PLAYER)->front());
            
            if (player)
            {
                player->SetFlyAwayEnding(true);
            }
        }
        i++;

        m_topY += 1;
        m_bottomY -= 1;

        if (m_topY > 400) m_topY = 400;
        if (m_bottomY < 300) m_bottomY = 300;

        if (m_topY == 400 && m_bottomY == 300)
        {
            m_bEndingCutScene = true;
            m_dwEndingSongDownVolume = GetTickCount();
        }
        
    }
}

void Scene03::EndingSongDownVolume()
{
    if (m_bEndingCutScene == true)
    {
        if (m_dwEndingSongDownVolume + 70 <= GetTickCount())
        {
            SOUND->SetChannelVolume(CHANNELID::SOUND_BGM, m_EndingSongVolume);
            m_EndingSongVolume -= 0.01f;
            m_dwEndingSongDownVolume = GetTickCount();
        }
    }
}
