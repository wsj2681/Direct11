#pragma once
#include "Polygon.h"
#include "Camera.h"

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

    unique_ptr<Camera>& GetCamera()
    {
        return this->camera;
    }

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<IDXGISwapChain> swapchain;
    ComPtr<ID3D11RenderTargetView> rtv;
    ComPtr<ID3D11DepthStencilView> dsv;
    
    unique_ptr<CPolygon> polygon;
    unique_ptr<Camera> camera;

public:

	void Initialize(HWND hWnd);
    void Render();

};