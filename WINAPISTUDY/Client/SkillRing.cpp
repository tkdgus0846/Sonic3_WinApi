#include "pch.h"
#include "SkillRing.h"
#include <ScrollManager.h>
#include <ImageObject.h>

SkillRing::SkillRing(int ringNum, const Vec2& pos)
{
    m_ZOrder = 1;
    m_Pos = pos;
    m_Size = { 32.f,32.f };
    m_RingsInfo.resize(ringNum);

    m_Frame.iFrameStart = 0;
    m_Frame.iFrameEnd = 15;
    m_Frame.iMotion = 0;
    m_Frame.dwTime = GetTickCount();
    m_Frame.dwSpeed = 10;
    m_Frame.repeat = true;

    m_fSpeed = 10.f;

    for (int i = 0; i < ringNum; i++)
    {
        int randX = rand() % 40;
        int randY = rand() % 20 - 10 + 1;
        int randSpeed = rand() % 10 + 10;
        m_RingsInfo[i].pos = pos;
        m_RingsInfo[i].dir = { (float)randX, (float)randY };
        m_RingsInfo[i].dir.Normalize();
        m_RingsInfo[i].speed = (float)randSpeed;
    }
}

SkillRing::~SkillRing()
{
}

void SkillRing::Render()
{
    for (SkillRingInfo& info : m_RingsInfo)
    {
        RENDER->Animation_TransparentBlt
        (
            L"Ring2",
            m_Frame,
            info.pos.x - 16,
            info.pos.y - 16,
            32,
            32,
            16,
            16,
            RGB(255, 0, 255),
            0, 0
        );
    }
}

void SkillRing::Init()
{
}

int SkillRing::Update()
{
    RingsOutWindow();
    if (GetDead()) return OBJ_DEAD;

    m_Pos.x = m_Pos.x + m_fSpeed;

    return OBJ_NOEVENT;
}

void SkillRing::Late_Update()
{
    SelectRingsPos();
    Move_Frame();
    CollisionWithObstacles();
    Update_Rect();
}

void SkillRing::SelectFrameKey()
{
}

void SkillRing::SelectRingsPos()
{
    for (SkillRingInfo& info : m_RingsInfo)
    {
        info.pos = info.pos + (info.dir * info.speed);
    }
}

void SkillRing::RingsOutWindow()
{
    int scrollX = (int)SCROLLX;

    if (m_Pos.x + scrollX > 1600)
    {
        SetDead();
    }
}

void SkillRing::CollisionWithObstacles()
{
    Scene* curScene = SCENE->GetCurrentScene();

    if (curScene)
    {
        list<GameObject*>* obsList = curScene->GetGameObjectslist(OBJ_OBSTACLE);
        if (obsList != nullptr)
        {
            RECT tmpRect;
            for (SkillRingInfo& info : m_RingsInfo)
            {
                RECT skillRect = { info.pos.x - 16, info.pos.y - 16 , info.pos.x + 16, info.pos.y + 16 };
                for (GameObject* obj : *obsList)
                {
                    ImageObject* obs = dynamic_cast<ImageObject*>(obj);
                    RECT objRect = obj->GetArea();

                    if (IntersectRect(&tmpRect, &skillRect, &objRect))
                        obs->Hit_With_SkillRing();
                }
            }
            
        }
        
    }
}
