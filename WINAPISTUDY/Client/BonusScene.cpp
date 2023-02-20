#include "pch.h"
#include "BonusScene.h"
#include <SoundManager.h>
#include "Ring.h"

void BonusScene::Init()
{
    Scene::Init();
    
    m_bRotating = false;

    m_ForwardFrame.iFrameStart = 0;
    m_ForwardFrame.iFrameEnd = 3;
    m_ForwardFrame.iMotion = 0;
    m_ForwardFrame.dwTime = GetTickCount();
    m_ForwardFrame.dwSpeed = 35;
    m_ForwardFrame.repeat = true;

    m_RotateFrame.iFrameStart = 0;
    m_RotateFrame.iFrameEnd = 3;
    m_RotateFrame.iMotion = 0;
    m_RotateFrame.dwTime = GetTickCount();
    m_RotateFrame.dwSpeed = 35;
    m_RotateFrame.repeat = false;

    m_SonicWalkFrame.iFrameStart = 0;
    m_SonicWalkFrame.iFrameEnd = 2;
    m_SonicWalkFrame.iMotion = 0;
    m_SonicWalkFrame.dwTime = GetTickCount();
    m_SonicWalkFrame.dwSpeed = 35;
    m_SonicWalkFrame.repeat = false;

    m_MyFrameKey = L"ForwardAndRotate";
    m_OtherFrameKey = L"ForwardAndRotate2";
    m_CurFrameKey = L"ForwardAndRotate";

    m_CurSonicWalkFrameKey = L"BonusSonicLeft";

    m_bSonicWalkReverse = false;

    m_SonicWalkImageLeftArr[0] = 0;
    m_SonicWalkImageLeftArr[1] = 50;
    m_SonicWalkImageLeftArr[2] = 99;

    m_SonicWalkImageRightArr[0] = 1;
    m_SonicWalkImageRightArr[1] = 49;
    m_SonicWalkImageRightArr[2] = 99;

    STOP_ALL_SOUND;
    PLAY_BGM(L"BonusStageBGM.wav", 1.0f);

    /*AddGameObject(OBJ_OBSTACLE, new Ring(7, L"Ring", 0, 0, {110, 200 }, Vec2(16 * 2, 16 * 2), { 16,16 }, true, 0));
    AddGameObject(OBJ_OBSTACLE, new Ring(7, L"Ring", 0, 0, { 300, 200 }, Vec2(16 * 2, 16 * 2), { 16,16 }, true, 0));
    AddGameObject(OBJ_OBSTACLE, new Ring(7, L"Ring", 0, 0, { 500, 200 }, Vec2(16 * 2, 16 * 2), { 16,16 }, true, 0));
    AddGameObject(OBJ_OBSTACLE, new Ring(7, L"Ring", 0, 0, { 690, 200 }, Vec2(16 * 2, 16 * 2), { 16,16 }, true, 0));*/
}

void BonusScene::Update()
{

    Scene::Update();
    


}

void BonusScene::Late_Update()
{
    Scene::Late_Update();
    
    MoveForwardFrame();
    MoveRotateFrame();
    MoveSonicWalkFrame();

    Key_Input();
}

void BonusScene::Render()
{
    BackgroundRender();
    RotateRender();
    ForwardRender();
    
    
    Scene::Render();
    

    cout << lstrcmp(m_CurSonicWalkFrameKey,L"BonusSonicLeft") << " " << m_SonicWalkFrame.iFrameStart << " "<< m_bSonicWalkReverse<< endl;
    SonicWalkRender();

}

void BonusScene::Release()
{
    Scene::Release();
    

}

void BonusScene::Forward()
{
}

// 320 196 8 8
void BonusScene::ForwardRender()
{
    if (m_bRotating == true) return;

    RENDER->Animation_TransparentBlt_NoScroll
    (
        m_MyFrameKey,
        m_ForwardFrame,
        0, 0,
        800, 600,
        320, 196,
        RGB(255, 0, 255),
        8, 8
    );
}

void BonusScene::SonicWalkRender()
{
    int posX;

    if (lstrcmp(m_CurSonicWalkFrameKey, L"BonusSonicLeft") == 0)
    {
        posX = m_SonicWalkImageLeftArr[m_SonicWalkFrame.iFrameStart];
    }
    else if (lstrcmp(m_CurSonicWalkFrameKey, L"BonusSonicRight") == 0)
    {
        posX = m_SonicWalkImageRightArr[m_SonicWalkFrame.iFrameStart];
    }
    
    RENDER->TransparentImage_NoScroll
    (
        m_CurSonicWalkFrameKey,
        327, 300,
        48 * 3, 53 * 3,
        posX,
        0,
        48, 53,
        RGB(255,0,255)
    );
}

void BonusScene::Rotate()
{
}

void BonusScene::RotateRender()
{
    if (m_bRotating == false) return;

    RENDER->Animation_TransparentBlt_NoScroll
    (
        m_CurFrameKey,
        m_RotateFrame,
        0, 0,
        800, 600,
        320, 196,
        RGB(255, 0, 255),
        8, 8
    );
}

void BonusScene::BackgroundRender()
{
    RENDER->StretchImage_NoScroll
    (
        L"BonusBackground",
        0, 0,
        800, 600,
        92, 0,
        341, 256
    );
}

void BonusScene::MoveSonicWalkFrame()
{
    if (m_bRotating == true) return;


    if (lstrcmp(m_CurSonicWalkFrameKey, L"BonusSonicLeft") == 0)
    {
        if (m_bSonicWalkReverse == false)
        {
            if (m_SonicWalkFrame.dwTime + m_SonicWalkFrame.dwSpeed < GetTickCount())
            {
                ++m_SonicWalkFrame.iFrameStart;
                if (m_SonicWalkFrame.iFrameStart > m_SonicWalkFrame.iFrameEnd)
                {
                    m_SonicWalkFrame.iFrameStart = 2;
                    m_bSonicWalkReverse = true;
                }
                m_SonicWalkFrame.dwTime = GetTickCount();
            }
        }
        else
        {
            if (m_SonicWalkFrame.dwTime + m_SonicWalkFrame.dwSpeed < GetTickCount())
            {
                --m_SonicWalkFrame.iFrameStart;
                if (m_SonicWalkFrame.iFrameStart < 0)
                {
                    m_SonicWalkFrame.iFrameStart = 2;
                    m_SonicWalkFrame.iFrameEnd = 0;
                    m_CurSonicWalkFrameKey = L"BonusSonicRight";
                    m_bSonicWalkReverse = false;
                }
                m_SonicWalkFrame.dwTime = GetTickCount();
            } 
        }
    }
    else if (lstrcmp(m_CurSonicWalkFrameKey, L"BonusSonicRight") == 0)
    {
        if (m_bSonicWalkReverse == false)
        {
            if (m_SonicWalkFrame.dwTime + m_SonicWalkFrame.dwSpeed < GetTickCount())
            {
                --m_SonicWalkFrame.iFrameStart;
                if (m_SonicWalkFrame.iFrameStart < m_SonicWalkFrame.iFrameEnd)
                {
                    m_SonicWalkFrame.iFrameStart = 0;
                    m_bSonicWalkReverse = true;
                }
                m_SonicWalkFrame.dwTime = GetTickCount();
            }
            
        }
        else
        {
            if (m_SonicWalkFrame.dwTime + m_SonicWalkFrame.dwSpeed < GetTickCount())
            {
                ++m_SonicWalkFrame.iFrameStart;
                if (m_SonicWalkFrame.iFrameStart > 2)
                {
                    m_SonicWalkFrame.iFrameStart = 0;
                    m_SonicWalkFrame.iFrameEnd = 2;
                    m_CurSonicWalkFrameKey = L"BonusSonicLeft";
                    m_bSonicWalkReverse = false;
                }
                m_SonicWalkFrame.dwTime = GetTickCount();
            }
        }
    }

    
        

     

    
    
    
}

void BonusScene::MoveForwardFrame()
{
    if (m_bRotating == true) return;

    if (m_ForwardFrame.dwTime + m_ForwardFrame.dwSpeed < GetTickCount())
    {
        ++m_ForwardFrame.iFrameStart;

        if (m_ForwardFrame.iFrameStart > m_ForwardFrame.iFrameEnd)
        {
            if (m_ForwardFrame.repeat == true)
            {
                m_ForwardFrame.iFrameStart = 0;
                m_ForwardFrame.iMotion++;
                if (m_ForwardFrame.iMotion > 3) m_ForwardFrame.iMotion = 0;
            }
            else if (m_ForwardFrame.repeat == false)
            {
                m_ForwardFrame.iFrameStart = m_ForwardFrame.iFrameEnd;
            }

        }

        m_ForwardFrame.dwTime = GetTickCount();
    }
}

void BonusScene::MoveRotateFrame()
{
    if (m_bRotating == false) return;

    if (m_eRotateDirection == RD_LEFT)
    {
        if (m_RotateFrame.dwTime + m_RotateFrame.dwSpeed < GetTickCount())
        {
            ++m_RotateFrame.iFrameStart;

            if (m_RotateFrame.iMotion == 5 && m_RotateFrame.iFrameStart == 3)
            {
                m_bRotating = false;
                return;
            }

            if (m_RotateFrame.iFrameStart > m_RotateFrame.iFrameEnd)
            {
                m_RotateFrame.iFrameStart = 0;
                m_RotateFrame.iMotion++;
            }

            m_RotateFrame.dwTime = GetTickCount();
        }
    }
    else
    {
        if (m_RotateFrame.dwTime + m_RotateFrame.dwSpeed < GetTickCount())
        {
            --m_RotateFrame.iFrameStart;

            if (m_RotateFrame.iMotion == 4 && m_RotateFrame.iFrameStart == -1)
            {
                m_bRotating = false;
                return;
            }

            if (m_RotateFrame.iFrameStart < m_RotateFrame.iFrameEnd)
            {
                m_RotateFrame.iFrameStart = 3;
                m_RotateFrame.iMotion--;

            }

            m_RotateFrame.dwTime = GetTickCount();
        }
    }

    
}

void BonusScene::Key_Input()
{
    if (INPUT->Key_Up(VK_RETURN)) SCENE->SelectScene("LogoScene");
    if (m_bRotating == false)
    {
        if (INPUT->Key_Up(VK_LEFT))
        {
            m_bRotating = true;
            m_eRotateDirection = RD_LEFT;
            m_RotateFrame.iMotion = 4;
            m_RotateFrame.iFrameStart = 0;
            m_RotateFrame.iFrameEnd = 3;

            if (m_ForwardFrame.iMotion == 0 || m_ForwardFrame.iMotion == 3)
            {
                m_CurFrameKey = m_MyFrameKey;
                m_ForwardFrame.iMotion = 2;
                m_ForwardFrame.iFrameStart = 1;
            }
            else
            {
                m_CurFrameKey = m_OtherFrameKey;
                m_ForwardFrame.iMotion = 0;
                m_ForwardFrame.iFrameStart = 1;
            }
        }
        else if (INPUT->Key_Up(VK_RIGHT))
        {
            m_bRotating = true;
            m_eRotateDirection = RD_RIGHT;
            m_RotateFrame.iMotion = 5;
            m_RotateFrame.iFrameStart = 2;
            m_RotateFrame.iFrameEnd = 0;

            if (m_ForwardFrame.iMotion == 0 || m_ForwardFrame.iMotion == 3)
            {
                m_CurFrameKey = m_OtherFrameKey;
                m_ForwardFrame.iMotion = 2;
                m_ForwardFrame.iFrameStart = 1;
            }
            else
            {
                m_CurFrameKey = m_MyFrameKey;
                m_ForwardFrame.iMotion = 0;
                m_ForwardFrame.iFrameStart = 1;
            }
        }
    }
    
}


