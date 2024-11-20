#include "framework.h"
#include "Core.h"


Core* Core::core = nullptr;
once_flag Core::initFlag;
HINSTANCE Core::initParam;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Core::CreateCore(hInstance);

    Core& coreApp = Core::getInstance();

    if (!coreApp.Initialize(nCmdShow))
    {
        return FALSE;
    }

    return coreApp.Run();
}