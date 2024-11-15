#pragma once
class DX11Device
{
public:
	DX11Device() = default;
    DX11Device(HWND hWnd);
	~DX11Device() = default;

    ComPtr<ID3D11Device> GetDevice()
    {
        return this->device;
    }
    ComPtr<ID3D11DeviceContext> GetDeviceContext()
    {
        return this->devcon;
    }
    ComPtr<IDXGISwapChain> GetSwapChain()
    {
        return this->swapchain;
    }

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<IDXGISwapChain> swapchain;
    ComPtr<ID3D11RenderTargetView> rtv;

    ComPtr<ID3D11Buffer> triangleVertexBuffer;
    ComPtr<ID3D11Buffer> triangleIndexBuffer;
    ComPtr<ID3D11VertexShader> trianglevertexShader;
    ComPtr<ID3D11PixelShader> trianglepixelShader;
    ComPtr<ID3D11InputLayout> triangleinputLayout;
public:

	void Initialize(HWND hWnd);
    void CreateTriangle();
    void Render();

};

//HR
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x) \
      if (FAILED((x))) \
      { \
         LPVOID errorLog = nullptr; \
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, \
            nullptr, (x), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
            reinterpret_cast<LPWSTR>(&errorLog), 0, nullptr); \
         fprintf(stderr, "%s", static_cast<char*>(errorLog)); \
         LocalFree(errorLog); \
         __debugbreak(); \
      }
#endif
#else
#ifndef HR
#define   HR(x) (x);
#endif
#endif