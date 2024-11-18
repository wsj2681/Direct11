#include "framework.h"
#include "DX11Device.h"


DX11Device::DX11Device(HWND hWnd)
{
	Initialize(hWnd);
}

void DX11Device::Initialize(HWND hWnd)
{
	// Create Device and SwapChain 
	DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
	swapchaindesc.BufferCount = 1;
	swapchaindesc.BufferDesc.Width = FRAMEBUFFER_WIDTH;
	swapchaindesc.BufferDesc.Height = FRAMEBUFFER_HEIGHT;
	swapchaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchaindesc.OutputWindow = hWnd;
	swapchaindesc.SampleDesc.Count = 1;
	swapchaindesc.Windowed = true;

	HR(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapchaindesc,
		swapchain.GetAddressOf(),
		device.GetAddressOf(),
		nullptr,
		devcon.GetAddressOf()
	));


	// Create RenderTargetView
	ComPtr<ID3D11Texture2D> backBuffer;
	HR(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	HR(device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv.GetAddressOf()));

	D3D11_TEXTURE2D_DESC depthStencildesc = {};
	depthStencildesc.Width = FRAMEBUFFER_WIDTH;
	depthStencildesc.Height = FRAMEBUFFER_HEIGHT;
	depthStencildesc.MipLevels = 1;
	depthStencildesc.ArraySize = 1;
	depthStencildesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencildesc.SampleDesc.Count = 1;
	depthStencildesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencildesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	HR(device->CreateTexture2D(&depthStencildesc, nullptr, depthStencilBuffer.GetAddressOf()));
	HR(device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, dsv.GetAddressOf()));

	devcon->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

	D3D11_RASTERIZER_DESC rasterdesc = {};
	rasterdesc.FillMode = D3D11_FILL_SOLID;
	rasterdesc.CullMode = D3D11_CULL_BACK;
	rasterdesc.FrontCounterClockwise = false;

	ComPtr<ID3D11RasterizerState> rasterState;
	HR(device->CreateRasterizerState(&rasterdesc, rasterState.GetAddressOf()));
	devcon->RSSetState(rasterState.Get());

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = FRAMEBUFFER_WIDTH;
	viewport.Height = FRAMEBUFFER_HEIGHT;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	devcon->RSSetViewports(1, &viewport);
}

void DX11Device::ClearBackBuffer()
{
	float clearColor[4] = { 0.f, 0.f, 0.4f, 1.f };
	devcon->ClearRenderTargetView(rtv.Get(), clearColor);
	devcon->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11Device::Render()
{
	swapchain->Present(1, 0);
}
