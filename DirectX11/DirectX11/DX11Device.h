#pragma once

class DX11Device
{
public:
	DX11Device() = default;
    DX11Device(HWND hWnd);
	~DX11Device() = default;

    ComPtr<ID3D11Device>& GetDevice()
    {
        return this->device;
    }
    ComPtr<ID3D11DeviceContext>& GetDeviceContext()
    {
        return this->devcon;
    }
    ComPtr<IDXGISwapChain>& GetSwapChain()
    {
        return this->swapchain;
    }

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<IDXGISwapChain> swapchain;
    ComPtr<ID3D11RenderTargetView> rtv;
    ComPtr<ID3D11DepthStencilView> dsv;

public:

	void Initialize(HWND hWnd);
    void ClearBackBuffer();
    void Render();

};