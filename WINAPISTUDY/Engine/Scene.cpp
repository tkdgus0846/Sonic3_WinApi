#include "pch.h"
#include "Scene.h"
#include "../Client/Player.h"
#include "ScrollManager.h"
#include "SoundManager.h"


Scene::Scene()
{
    m_bActClear = false;
}

Scene::~Scene()
{
    
}

void Scene::AddGameObject(int ObjID, GameObject* NewObj)
{
    /**/
    m_GameObjectList[ObjID].push_back(NewObj);
}

list<GameObject*>* Scene::GetGameObjectslist(int ObjID)
{
    return &m_GameObjectList[ObjID];
}

void Scene::RemoveGameObjectsList(int ObjID)
{
    for (auto it = m_GameObjectList[ObjID].begin(); it != m_GameObjectList[ObjID].end(); )
    {
        if (*it)
            Safe_Delete(&(*it));
        it = m_GameObjectList[ObjID].erase(it);      
    }
}


void Scene::Init()
{
    m_StartFrame.iFrameStart = 0;
    m_StartFrame.iFrameEnd = 2;
    m_StartFrame.dwTime = GetTickCount();
    m_StartFrame.iMotion = 0;
    m_StartFrame.dwSpeed = 30;
    m_StartFrame.repeat = false;
    m_bStartFrameEnded = false;

    m_dwStartTime = GetTickCount();
}

void Scene::Update()
{
    for (int i = 0; i < OBJ_END; i++)
    {
        for (auto it = m_GameObjectList[i].begin(); it != m_GameObjectList[i].end(); )
        {
            int isDead = (*it)->Update();
            if (isDead == OBJ_DEAD)
            {
                Safe_Delete(&(*it));
                it = m_GameObjectList[i].erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void Scene::Late_Update()
{
    for (int i = 0; i < OBJ_END; i++)
    {
        for (auto item : m_GameObjectList[i])
        {
            if (item)
            {
                item->Late_Update();

                if (item->IsObjectInWindow_Scroll() == true)
                    m_RenderQueue.push({ item });
            }
        }
    }
}

void Scene::Render()
{
    while (!m_RenderQueue.empty())
    {
        IRender* top = m_RenderQueue.top();

        if (top)
            top->Render();
        m_RenderQueue.pop();
    }

    /* 나중에 출력 할 것!*/
    /*
    if (!m_GameObjectList[OBJ_PLAYER].empty())
    {
        Player* player = dynamic_cast<Player*>(m_GameObjectList->front());
        if (player)
        {
            // StageStart 가 완료되고 난뒤 
            int rings = player->GetRings();
            int score = player->GetScore();
            int life = player->GetLife();

            RenderScoreRings(score, rings);
            RenderSonicLife(life);

        }
    }
    */
}

void Scene::Release()
{
    for (int i = 0; i < OBJ_END; i++)
    {
        for (auto it = m_GameObjectList[i].begin(); it != m_GameObjectList[i].end(); )
        {
            Safe_Delete(&*it);
            it = m_GameObjectList[i].erase(it);
        }
    }
}

void Scene::ActClear()
{
    if (m_GameObjectList[OBJ_PLAYER].empty()) return;
    Player* player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
    if (player)
    {
        player->SetActClear(true);
    }
    m_bActClear = true;
}

void Scene::ActClearEnded()
{
    if (m_GameObjectList[OBJ_PLAYER].empty()) return;
    Player* player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
    if (player)
    {
        player->SetActClear(false);
    }
    m_bActClear = false;
}

void Scene::Move_Frame()
{
    if (m_Frame.dwTime + m_Frame.dwSpeed < GetTickCount())
    {
        ++m_Frame.iFrameStart;

        if (m_Frame.iFrameStart > m_Frame.iFrameEnd)
        {
            if (m_Frame.repeat == true)
            {
                m_Frame.iFrameStart = 0;
            }
            else if (m_Frame.repeat == false)
            {
                m_Frame.iFrameStart = m_Frame.iFrameEnd;
            }

        }

        m_Frame.dwTime = GetTickCount();
    }
}

void Scene::Move_StartFrame()
{
    if (m_StartFrame.dwTime + m_StartFrame.dwSpeed < GetTickCount())
    {
        if (m_StartFrame.iFrameStart == 2 && m_StartFrame.iMotion == 4)
        {
            return;
        }

        ++m_StartFrame.iFrameStart;

        if (m_StartFrame.iFrameStart > m_StartFrame.iFrameEnd)
        {
            if (m_StartFrame.iMotion + 1 <= 4)
            {
                m_StartFrame.iMotion++;
                m_StartFrame.iFrameStart = 0;
            }
            else
            {
                m_StartFrame.iFrameStart = 2;
            }
        }

        m_StartFrame.dwTime = GetTickCount();
    }
}

// 48 25
void Scene::RenderScoreRings()
{
    RENDER->TransparentImage_NoScroll
    (
        L"UI",
        20, 25,
        48 * 2, 25 * 2,
        0, 0,
        48, 25,
        RGB(255, 0, 255)
    );

    if (m_GameObjectList[OBJ_PLAYER].empty() == false)
    {
        Player* player =dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
        if (player)
        {
            int rings = player->GetRings();
            int score = player->GetScore();

            RenderBigNumber(score, 150, 25);
            RenderBigNumber(rings, 120, 55);
        }
    }
}

// 48 25
void Scene::RenderSonicLife()
{
    RENDER->TransparentImage_NoScroll
    (
        L"UI",
        20, 545,
        48 * 2, 25 * 2,
        48, 0,
        48, 25,
        RGB(255, 0, 255)
    );

    if (m_GameObjectList[OBJ_PLAYER].empty() == false)
    {
        Player* player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
        if (player)
        {
            int life = player->GetLife();
            RenderSmallNumber(life, 80, 573);
        }
    }
}

// x는 8씩
void Scene::RenderBigNumber(int num, float x, float y)
{
    string str = to_string(num);
    int sep = 0;

	for (int i=0; i<str.length(); i++)
    {
        char ch = str[i];
        int printNum = ch - '0';
        
        RENDER->TransparentImage_NoScroll
        (
            L"Number",
            x + sep, y,
            8 * 2, 11 * 2,
            8 * printNum, 0,
            8, 11,
            RGB(255,0,255)
        );
        sep += 14;
    }
    
}

void Scene::RenderSmallNumber(int num, float x, float y)
{
    string str = to_string(num);
    int sep = 0;

    for (int i = 0; i < str.length(); i++)
    {
        char ch = str[i];
        int printNum = ch - '0';

        RENDER->TransparentImage_NoScroll
        (
            L"SmallNumber",
            x + sep, y,
            8 * 1.5, 6 * 2.0,
            8 * printNum, 0,
            8, 6,
            RGB(255, 0, 255)
        );
        sep += 11;
    }
}

void Scene::RenderStageStart(int stageNum)
{
    if (stageNum == 1)
    {
        /*RENDER->SetPen(RGB(0, 0, 0));
        RENDER->SetBrush(RGB(0, 0, 0));

        RENDER->DrawRect(0, 0, 800, 600);*/
        RENDER->Animation_TransparentBlt_NoScroll
        (
            L"Start1",
            m_StartFrame,
            0, 0,
            800, 600,
            320, 240,
            RGB(255,0,255),
            0, 0
        );
    }
    else if (stageNum == 2)
    {
        /*RENDER->SetPen(RGB(0, 0, 0));
        RENDER->SetBrush(RGB(0, 0, 0));

        RENDER->DrawRect(0, 0, 800, 600);*/
        RENDER->Animation_TransparentBlt_NoScroll
        (
            L"Start2",
            m_StartFrame,
            0, 0,
            800, 600,
            320, 240,
            RGB(255, 0, 255),
            0, 0
        );
    }

}

void Scene::RenderStageEnd(int stageNum)
{
    Player* player = nullptr;
    if (m_GameObjectList[OBJ_PLAYER].empty() == false)
    {
        player = dynamic_cast<Player*>(m_GameObjectList[OBJ_PLAYER].front());
    }

    if (stageNum == 1)
    {
        RENDER->TransparentImage_NoScroll
        (
            L"End1",
            150, 100,
            211 * 2.5, 117 * 2.5,
            0, 0,
            211, 117,
            RGB(255, 0, 255)
        );

        if (player == nullptr) return;
        if (player)
        {
            static int firstRings = player->GetRings();
            int rings = player->GetRings();
            int score = player->GetScore();
            static int total;
            
            if (rings != 0)
            {
                if (player->GetScore() == 0)
                {
                    player->SubRings(1);
                    total += 100;
                }
                else
                {
                    player->SubScore(100);
                    total += 100;
                }
            }
            else
            {
                player->AddScore(total);
                player->AddRings(firstRings);
                ActClearEnded();
                SCROLL_SHAKE;
            }
            
                
            RenderBigNumber(score, 550, 246);
            RenderBigNumber(rings * 100, 550, 296);
            RenderBigNumber(total, 550, 376);
        }
    }
    else if (stageNum == 2)
    {
        RENDER->TransparentImage_NoScroll
        (
            L"End2",
            150, 100,
            211 * 2.5, 117 * 2.5,
            0, 0,
            211, 117,
            RGB(255, 0, 255)
        );

        if (player == nullptr) return;
        if (player)
        {
            static int i = 0;

            if (i == 0)
            {
                STOP_SOUND(CHANNELID::SOUND_BGM);
                STOP_PLAY_SOUND(L"ActClear.wav", CHANNELID::SOUND_EFFECT, 1.0f);
            }
            i++;
            static int firstRings = player->GetRings();
            int rings = player->GetRings();
            int score = player->GetScore();
            static int total;

            if (rings != 0)
            {
                if (player->GetScore() == 0)
                {
                    player->SubRings(1);
                    total += 100;
                }
                else
                {
                    player->SubScore(100);
                    total += 100;
                }
            }
            else
            {
                player->AddScore(total);
                player->AddRings(firstRings);
                ActClearEnded();
                SCENE->SelectScene("Scene03");
                return;
                SCROLL_UNLOCK;
            }


            RenderBigNumber(score, 550, 246);
            RenderBigNumber(rings * 100, 550, 296);
            RenderBigNumber(total, 550, 376);
        }
    }
}

