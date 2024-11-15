#pragma once
#include "Polygon.h"
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

    ComPtr<ID3D11Buffer> rectVertexBuffer;
    ComPtr<ID3D11Buffer> rectIndexBuffer;
    ComPtr<ID3D11VertexShader> rectvertexShader;
    ComPtr<ID3D11PixelShader> rectpixelShader;
    ComPtr<ID3D11InputLayout> rectinputLayout;

    unique_ptr<CPolygon> polygon;

public:

	void Initialize(HWND hWnd);
    void Render();

};