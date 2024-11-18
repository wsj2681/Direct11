#pragma once
#include "DX11Device.h"
#include "Polygon.h"

#define MAX_LOADSTRING 100

class Core
{
public:
    Core(HINSTANCE hInstance);
    ~Core();

    bool Initialize(int nCmdShow);
    int Run();

protected:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    bool RegisterWindowClass();
    bool InitInstance(int nCmdShow);

    HINSTANCE hInstance;
    HWND hWnd;
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
    
    POINT lastMousePosition = {};

    unique_ptr<DX11Device> device;
};