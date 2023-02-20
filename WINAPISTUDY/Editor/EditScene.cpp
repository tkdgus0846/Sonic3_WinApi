#include "pch.h"
#include "EditScene.h"

#include "LineManager.h"
#include "Line.h"
#include "SceneManager.h"
#include "TileManager.h"
#include "ScrollManager.h"

void EditScene::Init()
{
    Scene::Init();
    drawmode = DM_TILE;
    bShowCoord = false;

    selectMode = DRAW;
    m_FrameKeyArr[0] = L"MushroomTile";
    m_FrameKeyArr[1] = L"LavareefTile";

    m_NowFrameKeyIndex = 0;
    m_NowCoordX = 0;
    m_NowZOrder = 3;
    m_NowCoordY = 0;
    m_NowSize = 256.f;
    m_SplitMode = 0;
    m_NowSpikeNum = 0;
    m_NowItemNum = 0;

   
    SCROLL->SetScrollX(630 * 64 * -1);
    SCROLL->SetScrollY(284 * 64 * -1);
    //AddGameObject(OBJ_PLAYER, FACTORY_CREATE(Player, Vec2(134 * 64, -72 * 64)));
}

void EditScene::Update()
{
    
    Scene::Update();
    LINE_MANAGER->Update();
    TILE->Update();
}

void EditScene::Late_Update()
{
    KeyInput();
    MouseInput();
    Scene::Late_Update();
    
    

   
}

void EditScene::Render()
{
    // 스테이지 1 배경
    /*RENDER->BitBltImage
    (L"MushroomBack",
        -100, -100,
        20, 10,
        -SCROLLX, -SCROLLY
    );*/
    // 1024 768
   
    /*LONG x = -SCROLLX;
    LONG y = -SCROLLY;

    RENDER->StretchImage
    (
        L"MushroomBack",
        -SCROLLX, -SCROLLY,
        800, 600,
        x % 722, y % 532,
        302, 236
    );*/
    Scene::Render();
    LINE_MANAGER->Render();
    COLLISION->Render();
    TILE->Render();

    

   
    RenderCoord();
    RenderHotKey();
}

void EditScene::Release()
{
    Scene::Release();
    LINE_MANAGER->Release();
    TILE->Release();

}

void EditScene::SelectFrameKey()
{
}

void EditScene::KeyInput()
{
    if (INPUT->Key_Down(VK_K))
    {
        LINE_MANAGER->Save_File(L"../Data/Line2.dat");
        TILE->Save_File(L"../Data/Tile2.dat");
        SCENE->Save_File(L"../Data/ImageObject2.dat");
    }
    if (INPUT->Key_Down(VK_L))
    {
        LINE_MANAGER->Load_File(L"../Data/Line2.dat");
        TILE->Load_File(L"../Data/Tile2.dat");
        SCENE->Load_File(L"../Data/ImageObject2.dat");
    }

    if (INPUT->Key_Up(VK_1))
    {
        drawmode = DM_TILE;
    }
    if (INPUT->Key_Up(VK_2))
    {
        drawmode = DM_FLINE;
    }
    if (INPUT->Key_Up(VK_3))
    {
        drawmode = DM_RLINE;
    }

    if (INPUT->Key_Up(VK_4))
    {
        if (m_NowFrameKeyIndex == 0) m_NowFrameKeyIndex = 1;
        else m_NowFrameKeyIndex = 0;
    }
    if (INPUT->Key_Up(VK_5))
    {
        if (m_NowZOrder == 1) m_NowZOrder = 3;
        else m_NowZOrder = 1;
    }
    if (INPUT->Key_Up(VK_6))
    {
        m_NowCoordX -= 1;

        if (m_NowCoordX <= -1)
            m_NowCoordX = 7;
    }
    if (INPUT->Key_Up(VK_7))
    {
        m_NowCoordX += 1;

        if (m_NowCoordX >= 8)
            m_NowCoordX = 0;
    }

    if (INPUT->Key_Up(VK_8))
    {
        m_NowCoordY -= 1;

        if (m_NowCoordY <= -1)
            m_NowCoordY = 11;
    }
    if (INPUT->Key_Up(VK_9))
    {
        m_NowCoordY += 1;

        if (m_NowCoordY >= 12)
            m_NowCoordY = 0;
    }

    if (INPUT->Key_Up(VK_NUMPAD1))
    {
        drawmode = DM_NUM1;
        m_NowCoordY = 0;
    }
    if (INPUT->Key_Up(VK_NUMPAD2))
    {
        drawmode = DM_NUM2;
        m_NowCoordY = 1;
    }
    if (INPUT->Key_Up(VK_NUMPAD3))
    {
        drawmode = DM_NUM3;
        m_NowCoordY = 2;
    }
    if (INPUT->Key_Up(VK_NUMPAD4))
    {
        drawmode = DM_NUM4;
        m_NowCoordY = 3;
    }
    if (INPUT->Key_Up(VK_NUMPAD5))
    {
        drawmode = DM_NUM5;
        m_NowCoordY = 4;
    }
    if (INPUT->Key_Up(VK_NUMPAD6))
    {
        drawmode = DM_NUM6;
        m_NowCoordY = 5;
    }
    if (INPUT->Key_Up(VK_NUMPAD7))
    {
        drawmode = DM_NUM7;
        m_NowCoordY = 6;
    }
    if (INPUT->Key_Up(VK_NUMPAD8))
    {
        drawmode = DM_NUM8;
        m_NowCoordY = 7;
    }
    if (INPUT->Key_Up(VK_NUMPAD9))
    {
        drawmode = DM_NUM9;
        m_NowCoordY = 8;
    }
    if (INPUT->Key_Up(VK_DIVIDE))
    {
        drawmode = DM_DIVIDE;
        m_NowCoordY = 9;
    }
    if (INPUT->Key_Up(VK_MULTIPLY))
    {
        drawmode = DM_PRODUCT;
        m_NowCoordY = 10;
    }
    if (INPUT->Key_Up(VK_SUBTRACT))
    {
        drawmode = DM_MINUS;
        m_NowCoordY = 11;
    }

    // 왼쪽 대괄호
    if (INPUT->Key_Up(VK_OEM_4))
    {
        m_NowSize /= 2.f;
        if (m_NowSize <= 32.f)
            m_NowSize = 64.f;
    }
    // 오른쪽 대괄호
    if (INPUT->Key_Up(VK_OEM_6))
    {
        m_NowSize *= 2.f;
        if (m_NowSize > 256.f)
            m_NowSize = 256.f;
    }
    

    if (INPUT->Key_Up(VK_E))
    {
        drawmode = DM_SPIKE;
        m_NowSpikeNum += 1;

        if (m_NowSpikeNum > 2)
            m_NowSpikeNum = 0;
    }

    if (INPUT->Key_Up(VK_R))
    {
        drawmode = DM_ROLLER;
    }

    if (INPUT->Key_Up(VK_T))
    {
        drawmode = DM_RING;
    }

    if (INPUT->Key_Up(VK_F))
    {
        drawmode = DM_SPRING;
    }

    if (INPUT->Key_Up(VK_G))
    {
        drawmode = DM_DIAGONALSPRING;
    }

    if (INPUT->Key_Up(VK_H))
    {
        drawmode = DM_MUSHROOM;
    }

    if (INPUT->Key_Up(VK_Z))
    {
        drawmode = DM_BGMUSH1;
    }
    if (INPUT->Key_Up(VK_X))
    {
        drawmode = DM_BGMUSH2;
    }
    if (INPUT->Key_Up(VK_C))
    {
        drawmode = DM_BGMUSH3;
    }

    if (INPUT->Key_Up(VK_R))
    {
        drawmode = DM_ROLLER;
    }

    if (INPUT->Key_Up(VK_T))
    {
        drawmode = DM_RING;
    }

    if (INPUT->Key_Up(VK_Y))
    {
        drawmode = DM_ITEM;
        m_NowItemNum += 1;
        if (m_NowItemNum == 4) m_NowItemNum = 0;
    }

    if (INPUT->Key_Up(VK_U))
    {
        drawmode = DM_STONE;
    }

    if (INPUT->Key_Up(VK_0))
    {
        drawmode = DM_BGBLACK;
    }

    if (INPUT->Key_Up(VK_I))
    {
        if (drawmode == DM_CLOUD1) drawmode = DM_CLOUD2;
        else drawmode = DM_CLOUD1;
    }
    if (INPUT->Key_Up(VK_O))
    {
        drawmode = DM_CLOUDTILE;
    }
    if (INPUT->Key_Up(VK_P))
    {
        drawmode = DM_PIPE;
    }
   

    // 스크롤 조정 키
    if (INPUT->Key_Pressing(VK_W))
    {
        SCROLL->SetScrollY(50.f);
    }
    if (INPUT->Key_Pressing(VK_S))
    {
        SCROLL->SetScrollY(-50.f);
    }
    if (INPUT->Key_Pressing(VK_A))
    {
        SCROLL->SetScrollX(50.f);
    }
    if (INPUT->Key_Pressing(VK_D))
    {
        SCROLL->SetScrollX(-50.f);
    }

    

    if (INPUT->Key_Up(VK_V))
    {
        if (selectMode == DRAW)
            selectMode = ERASE;
        else if (selectMode == ERASE)
            selectMode = DRAW;
    }
    if (INPUT->Key_Up(VK_B))
    {
        if (m_SplitMode == 0)
        {
            m_NowSize = 128.f;
            m_SplitMode = 1;
        }
        else
        {
            m_SplitMode++;
            if (m_SplitMode == 5)
            {
                m_SplitMode = 0;
                m_NowSize = 256.f;
            }
        }
    }

    if (INPUT->Key_Up(VK_SPACE))
    {
        if (bShowCoord == true)
        {
            bShowCoord = false;
        }
        else
        {
            bShowCoord = true;
        }
    }
}

void EditScene::MouseInput()
{

    switch (drawmode)
    {
    case DM_TILE:
        TILE->SetSelectMode(selectMode);
        TILE->Mouse_Input();
        break;
    case DM_FLINE:
        LINE_MANAGER->SetSelectMode(selectMode, LL_FORWARD);
        LINE_MANAGER->Mouse_Input();
        break;
    case DM_RLINE:
        LINE_MANAGER->SetSelectMode(selectMode, LL_REVERSE);
        LINE_MANAGER->Mouse_Input();
        break;
    case DM_SPIKE:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Spike", 4, m_NowSpikeNum, 0, Vec2(m_NowSize,m_NowSize), true);
        break;
    case DM_ROLLER:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Roller", 4, 0, 0, Vec2(66*1.7,13*1.7), true, 0, true);
        break;
    case DM_RING:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Ring2", 4, 0, 0, Vec2(16 * 2, 16 * 2), true, 0, true);
        break;
    case DM_SPRING:
        if (m_NowCoordY > 3) break;
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Spring", 4, m_NowFrameKeyIndex * 2, m_NowCoordY, Vec2(32 * 2.5, 32 * 2.5), true, 0, true);
        break;
    case DM_DIAGONALSPRING:
        if (m_NowCoordY > 3) break;
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"DiagonalSpring", 4, m_NowFrameKeyIndex * 2, m_NowCoordY, Vec2(36 * 2.5, 36 * 2.5), true, 1, true);
        break;
    case DM_BGMUSH1:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"BGMushroom1", m_NowZOrder, 0, 0, Vec2(72 * 2, 40 * 2), true, 0, true);
        break;
    case DM_BGMUSH2:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"BGMushroom2", m_NowZOrder, 0, 0, Vec2(16 * 2, 27 * 2), true, 0, true);
        break;
    case DM_BGMUSH3:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"BGMushroom3", m_NowZOrder, 0, 0, Vec2(16 * 2, 32 * 2), true, 0, true);
        break;
    case DM_MUSHROOM:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Mushroom", m_NowZOrder, 0, 0, Vec2(70 * 2, 60 * 2), true, 0, true);
        break;
    case DM_ITEM:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Item", m_NowZOrder, 0, m_NowItemNum, Vec2(28 * 2, 32 * 2), true, 0, true);
        break;
    case DM_STONE:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Stone1", 6, 0, m_NowItemNum, Vec2(32 * 2, 29 * 2), true, 0, true, true);
        break;
    case DM_BGBLACK:
        SCENE->SetSelectMode(DRAW, SIOM_IMAGE);
        SCENE->Mouse_Input(L"LavareefTile", -1, 2, 4, Vec2(256, 256), false, 0, true, true);
        break;
    case DM_CLOUD1:
        SCENE->SetSelectMode(selectMode, SIOM_IMAGE);
        SCENE->Mouse_Input(L"Cloud1", m_NowZOrder, 0, 0, Vec2(32*3, 24*3), false, 0, false, false);
        break;
    case DM_CLOUD2:
        SCENE->SetSelectMode(selectMode, SIOM_IMAGE);
        SCENE->Mouse_Input(L"Cloud2", m_NowZOrder, 0, 0, Vec2(64*3, 24*3), false, 0, false, false);
        break;
    case DM_CLOUDTILE:
        SCENE->SetSelectMode(selectMode, SIOM_IMAGE);
        SCENE->Mouse_Input(L"CloudTile", m_NowZOrder, 0, 0, Vec2(16*4, 16*4), false, 0, false, false);
        break;
    case DM_PIPE:
        SCENE->SetSelectMode(selectMode, SIOM_OBSTACLE);
        SCENE->Mouse_Input(L"Pipe", m_NowZOrder, 0, 0, Vec2(32*4.2, 32*4.5), true, 0, true, true);
        break;
    default:
        SCENE->SetSelectMode(selectMode, SIOM_IMAGE);
        SCENE->Mouse_Input(m_FrameKeyArr[m_NowFrameKeyIndex], m_NowZOrder, m_NowCoordX, m_NowCoordY, Vec2(m_NowSize, m_NowSize), false, m_SplitMode);
        break;
    }
}

void EditScene::RenderCoord()
{
    RENDER->SetText(10, RGB(0, 0, 0));
    RENDER->SetTextBackMode(TEXTBACKMODE::Null);

    if (bShowCoord == true)
    {
        // 스크롤에 따른 좌표를 보여줘야 함


        int x = -(int)SCROLLX;
        int y = -(int)SCROLLY;
        float maxX = x + 800;
        float maxY = y + 600;

        for (int tmpX = x; tmpX < maxX; tmpX += 1)
        {
            for (int tmpY = y; tmpY < maxY; tmpY += 1)
            {
                int xCoord = tmpX / 64;
                int yCoord = tmpY / 64;
                int isX = abs((int)tmpX) % 64;
                int isY = abs((int)tmpY) % 64;

                if (isX == 0 && isY == 0)
                {
                    wstring text;
                    text = text + to_wstring(xCoord);
                    text = text + L" , ";
                    text = text + to_wstring(yCoord);

                    RENDER->Text(tmpX + (int)SCROLLX, tmpY + (int)SCROLLY, text);
                }

            }
        }
    }
}

void EditScene::RenderHotKey()
{
    //RENDER->SetText(20, RGB(50, 50, 50));
    //RENDER->SetTextBackMode(TEXTBACKMODE::Null);

    switch (drawmode)
    {
    case DM_TILE:
        RENDER->Text(700, 100, L"타일");
        break;
    case DM_FLINE:
        RENDER->Text(700, 100, L"정방향 선");
        break;
    case DM_RLINE:
        RENDER->Text(700, 100, L"역방향 선");
        break;
    case DM_SPIKE:
        RENDER->Text(700, 100, to_wstring(m_NowSpikeNum + 1)+L" 가시");
        break;
    case DM_ROLLER:
        RENDER->Text(700, 100, L"롤러");
        break;
    case DM_RING:
        RENDER->Text(700, 100, L"링");
        break;
    case DM_SPRING:
        RENDER->Text(700, 100, L"스프링");
        break;
    case DM_DIAGONALSPRING:
        RENDER->Text(700, 100, L"대각 스프링");
        break;
    case DM_BGMUSH1:
        RENDER->Text(700, 100, L"배경버섯1");
        break;
    case DM_BGMUSH2:
        RENDER->Text(700, 100, L"배경버섯2");
        break;
    case DM_BGMUSH3:
        RENDER->Text(700, 100, L"배경버섯3");
        break;
    case DM_MUSHROOM:
        RENDER->Text(700, 100, L"점프버섯");
        break;
    case DM_ITEM:
        RENDER->Text(700, 100, L"아이템: " + to_wstring(m_NowItemNum + 1));
        break;
    case DM_STONE:
        RENDER->Text(700, 100, L"부서지는돌");
        break;
    case DM_BGBLACK:
        RENDER->Text(700, 100, L"검은색배경");
        break;
    case DM_CLOUD1:
        RENDER->Text(700, 100, L"구름1");
        break;
    case DM_CLOUD2:
        RENDER->Text(700, 100, L"구름2");
        break;
    case DM_CLOUDTILE:
        RENDER->Text(700, 100, L"구름타일");
        break;
    case DM_PIPE:
        RENDER->Text(700, 100, L"파이프");
        break;
    default:
        RENDER->Text(700, 100,L"("+ to_wstring(m_NowCoordY+1) + L"," + to_wstring(m_NowCoordX+1)+L")");
        break;

    }

    switch (m_SplitMode)
    {
    case 0:
        RENDER->Text(700, 125, L"스플릿모드 X");
        break;
    case 1:
        RENDER->Text(700, 125, L"스플릿모드 1");
        break;
    case 2:
        RENDER->Text(700, 125, L"스플릿모드 2");
        break;
    case 3:
        RENDER->Text(700, 125, L"스플릿모드 3");
        break;
    case 4:
        RENDER->Text(700, 125, L"스플릿모드 4");
        break;
    }

    switch (selectMode)
    {
    case DRAW:
        RENDER->Text(700, 150, L"그리기모드");
        break;
    case ERASE:
        RENDER->Text(700, 150, L"지우기모드");
        break;
    }

    RENDER->Text(700, 175, to_wstring((int)m_NowSize)+L"X"+ to_wstring((int)m_NowSize));

    switch (m_NowFrameKeyIndex)
    {
    case 0:
        RENDER->Text(700, 200, L"스테이지1");
        break;
    case 1:
        RENDER->Text(700, 200, L"스테이지2");
        break;
    }

    RENDER->Text(700, 225, L"(" + to_wstring(m_NowCoordY + 1) + L"," + to_wstring(m_NowCoordX + 1) + L")");

    switch (m_NowZOrder)
    {
    case 3:
        RENDER->Text(700, 250, L"플레이어 앞에");
        break;
    case 1:
        RENDER->Text(700, 250, L"플레이어 뒤에");
        break;
    }

    

    RENDER->Text(650, 350, L"1: 타일");
    RENDER->Text(650, 375, L"2: 정방향 라인");
    RENDER->Text(650, 400, L"3: 역방향 라인");
    RENDER->Text(650, 425, L"4: 스테이지 변경");
    RENDER->Text(650, 450, L"5: 렌더 순서 변경");
    RENDER->Text(650, 475, L"6/7: 이미지 x좌표 -/+");
    RENDER->Text(650, 500, L"넘패드: 이미지 y좌표 변경");
    RENDER->Text(650, 525, L"E,R,T: 가시");
    RENDER->Text(650, 550, L"SPACE: 좌표표시");
    RENDER->Text(650, 575, L"V: 그리기/지우기");
    
    
}


