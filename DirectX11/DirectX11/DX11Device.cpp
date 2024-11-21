#include "framework.h"
#include "DX11Device.h"


DX11Device::DX11Device(HWND hWnd)
{
	Initialize(hWnd);
}

DX11Device::~DX11Device()
{
	ComPtr<ID3D11Debug> debugInterface;
	if (SUCCEEDED(device.As(&debugInterface))) 
	{
		debugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}

	SAFE_RESET(rtv);
	SAFE_RESET(dsv);
	SAFE_RESET(devcon);
	SAFE_RESET(device);
	SAFE_RESET(swapchain);
}

void DX11Device::Initialize(HWND hWnd)
{
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create Device and SwapChain 
	DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
	swapchaindesc.BufferCount = 2;
	swapchaindesc.BufferDesc.Width = FRAMEBUFFER_WIDTH;
	swapchaindesc.BufferDesc.Height = FRAMEBUFFER_HEIGHT;
	swapchaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchaindesc.OutputWindow = hWnd;
	swapchaindesc.SampleDesc.Count = 1;
	swapchaindesc.Windowed = true;
	swapchaindesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	HR(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
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

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> depthStencilState;
	HR(device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf()));
	devcon->OMSetDepthStencilState(depthStencilState.Get(), 1);

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

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencildesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, dsv.GetAddressOf()));

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
	devcon->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

	float clearColor[4] = { 1.f, 1.f, 1.f, 1.f };
	devcon->ClearRenderTargetView(rtv.Get(), clearColor);
	devcon->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11Device::Render()
{
	swapchain->Present(1, 0);
}
