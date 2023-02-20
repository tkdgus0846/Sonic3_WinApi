#include "pch.h"
#include "Game.h"

#include "Collider.h"

#include "AbstractFactory.h"
#include "EditScene.h"
#include "LineManager.h"
#include "ScrollManager.h"
#include "ResourceManager.h"
#include <TileManager.h>

#define MISSILE_GENERATION_TIMER    1

LRESULT CALLBACK Game::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        //SetTimer(hWnd, MISSILE_GENERATION_TIMER, 100, 0);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MISSILE_GENERATION_TIMER:
            /*GenMissile(); */
            return DefWindowProc(hWnd, message, wParam, lParam);
        }    
        break;
    }
    case WM_COMMAND:
        return DefWindowProc(hWnd, message, wParam, lParam);
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

Game::Game() :
    m_windowRect({ 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }),
    m_szTitle(L"MyGame"),
    m_szWindowClass(L"MyGameClass")
{
}

Game::~Game()
{
}

BOOL Game::Init(HINSTANCE hInstance, int CmdShow)
{ 
    m_hInst = hInstance;
    
    AdjustWindowRect(&m_windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(
        m_szWindowClass, 
        m_szTitle, 
        WS_OVERLAPPED | WS_SYSMENU,
        CW_USEDEFAULT, 
        0, 
        m_windowRect.right - m_windowRect.left, 
        m_windowRect.bottom - m_windowRect.top, 
        nullptr, 
        nullptr, 
        hInstance, 
        nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    m_hWnd = hWnd;
    ShowWindow(hWnd, CmdShow);
    UpdateWindow(hWnd);

    
    
    /* -------------- 매니저 Init ----------------------*/
    INPUT->Init();
    RANDOM->Init();
    RENDER->Init(m_hWnd);
    TIME->Init();
    SCROLL->Init();
    RESOURCE->Init();
    LINE_MANAGER->Init(m_hWnd);
    TILE->Init(m_hWnd);
    SCENE->Init(m_hWnd);

    // Scene들 초기화
    SCENE->AddScene("EditScene", new EditScene());
    SCENE->SelectScene("EditScene");

    return TRUE;
}

void Game::Run()
{
    Input();
    Update();
    Late_Update();
    Render();
}

void Game::Release()
{
    

    // 매니져들 release
    SCENE->Release();
    RENDER->Release(); 
    RESOURCE->Release();
}

void Game::Update()
{
    // 매니져 업데이트.
    TIME->Update();
    SCENE->Update();
}

void Game::Late_Update()
{
    // 매니져 레이트 업데이트.
    SCENE->Late_Update();    
}

void Game::Input()
{
    INPUT->Update();
}

void Game::Render()
{
    RENDER->BeginDraw();
    
    // 렌더 기능이 있는 매니져들 렌더링
    SCENE->Render(); 
    
    
    
    RENDER->EndDraw();
}

// 화면의 안보이는 곳에서 미사일을 생성한다.

