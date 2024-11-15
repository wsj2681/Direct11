#include "framework.h"
#include "Core.h"
#include "resource.h"

Core::Core(HINSTANCE hInstance)
    : hInstance(hInstance), hWnd(nullptr)
{
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTX11, szWindowClass, MAX_LOADSTRING);
}

Core::~Core() {}

bool Core::Initialize(int nCmdShow)
{
    if (!RegisterWindowClass())
    {
        return false;
    }
    if (!InitInstance(nCmdShow))
    {
        return false;
    }

    device = make_unique<DX11Device>(hWnd);

    return true;
}

int Core::Run()
{
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            device->Render();
        }
    }
    return static_cast<int>(msg.wParam);
}

bool Core::RegisterWindowClass()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_DIRECTX11));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

bool Core::InitInstance(int nCmdShow)
{
    RECT rc = { 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT };

    hWnd = CreateWindowW(
        szWindowClass, 
        szTitle, 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 
        0, 
        rc.right - rc.left, 
        rc.bottom - rc.top, 
        nullptr, 
        nullptr, 
        hInstance, 
        nullptr);

    if (!hWnd)
    {
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return true;
}

LRESULT CALLBACK Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}