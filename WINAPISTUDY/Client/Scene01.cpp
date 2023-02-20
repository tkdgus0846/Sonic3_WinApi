#include "pch.h"
#include "Scene01.h"
#include "Player.h"
#include "LineManager.h"
#include "Line.h"
#include "SceneManager.h"
#include "TileManager.h"
#include <ScrollManager.h>
#include <SoundManager.h>
#include "Sign.h"

void Scene01::Init()
{
    Scene::Init();
    LINE_MANAGER->Load_File(L"../Data/Line1.dat");
    TILE->Load_File(L"../Data/Tile1.dat");
    SCENE->Load_File(L"../Data/ImageObject1.dat");
    
    Player* player = new Player(Vec2(300, 400));
    player->Init();
    /*player->SetSuperMode();*/
    AddGameObject(OBJ_PLAYER, player);
    //AddGameObject(OBJ_PLAYER, FACTORY_CREATE(Player, Vec2(7232, -2373)));
    /*AddGameObject(OBJ_PLAYER, FACTORY_CREATE(Player, Vec2(134*64, -72*64)));*/
    STOP_ALL_SOUND;
    PLAY_BGM(L"MushroomHillZone2.wav", 1.f);
    m_fEndPointX = 17408.f;
    m_fEndPointY = -1536.f;

    m_fNextStageX = 312 * 64;

    /*AddGameObject(OBJ_PLAYER, FACTORY_CREATE(Player, Vec2(m_fEndPointX -400, m_fEndPointY + 300)));*/

    m_bSign = false;

    dwTime = GetTickCount();
}

void Scene01::Update()
{
    Scene::Update();
    LINE_MANAGER->Update();
    TILE->Update();
}

void Scene01::Late_Update()
{
    Scene::Late_Update();
    COLLISION->Late_Update();
    COLLISION->Collision_Player_Line();
    COLLISION->Collision_Object_Line();
    COLLISION->Collision_With_Tile();
    COLLISION->Collision_With_Obstacle("PlayerBodyCollider", GetGameObjectslist(OBJ_OBSTACLE));
    COLLISION->Collision_With_ObstacleCollider("PlayerBodyCollider", GetGameObjectslist(OBJ_OBSTACLE));

    if (m_bSign == false)
    {
        GameObject* player = m_GameObjectList[OBJ_PLAYER].front();
        if (player)
        {
            int x = player->GetPos().x;
            //cout << x << " " << m_fEndPointX << endl;
            if (x >= m_fEndPointX)
            {
                //cout << "Appeard!" << endl;
                AddGameObject(OBJ_OBSTACLE, new Sign(2, L"Sign", 0, 0, { m_fEndPointX - 100,m_fEndPointY }, { 48 * 2,47 * 2 }, { 48,47 }, true));
                SCROLL_LOCK;
                m_bSign = true;
            }
           
        }
    }
    else
    {
        GameObject* player = m_GameObjectList[OBJ_PLAYER].front();
        if (player)
        {
            int x = player->GetPos().x;
            
            if (x >= m_fNextStageX)
            {
                SCENE->SelectScene("Scene02");
            }
        }
    }
    

    Move_StartFrame();
    
}

void Scene01::Render()
{
    // 스테이지 1 배경
    /*RENDER->BitBltImage
    (L"MushroomBack",
        -100, -100,
        20, 10,
        -SCROLLX, -SCROLLY
    );*/
    // 1024 768
    int      ScrollX = (int)SCROLLX;
    int      ScrollY = (int)SCROLLY;    

    // 1024 768
    RENDER->StretchImage_NoScroll
    (
        L"MushroomBack",
        0, 0,
        800, 600,
        0 - ScrollX * 0.1f, 400 - ScrollY * 0.01f,
        302, 236
    );
    RENDER->StretchImage_NoScroll
    (
        L"MushroomBack",
        0, 0,
        800, 600,
        -900 - ScrollX * 0.1f, 400 - ScrollY * 0.01f,
        302, 236
    );
    RENDER->StretchImage_NoScroll
    (
        L"MushroomBack",
        0, 0,
        800, 600,
        -1500 - ScrollX * 0.1f, 400 - ScrollY * 0.01f,
        302, 236
    );
    Scene::Render();
    //LINE_MANAGER->Render();
    
    //TILE->Render();
    COLLISION->Render();

    if (m_bActClear == true)
    {
        RenderStageEnd(1);
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
        RenderStageStart(1);
    }
    else
    {
        m_bStartFrameEnded = true;
    }

    

        
}

void Scene01::Release()
{
    Scene::Release();
    LINE_MANAGER->Release();
    TILE->Release();

}



void Scene01::SelectFrameKey()
{
}




