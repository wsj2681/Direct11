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

	devcon->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = FRAMEBUFFER_WIDTH;
	viewport.Height = FRAMEBUFFER_HEIGHT;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	devcon->RSSetViewports(1, &viewport);

	CreateTriangle();
}

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

void DX11Device::CreateTriangle()
{
	Vertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	};

	D3D11_BUFFER_DESC bufferdesc= {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = sizeof(vertices);
	bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = vertices;

	HR(device->CreateBuffer(&bufferdesc, &initdata, triangleBuffer.GetAddressOf()));

	ComPtr<ID3DBlob> vsBlob;
	HR(D3DCompileFromFile(L"triangleShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, vsBlob.GetAddressOf(), nullptr));
	HR(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, trianglevertexShader.GetAddressOf()));

	ComPtr<ID3DBlob> psBlob;
	HR(D3DCompileFromFile(L"triangleShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, psBlob.GetAddressOf(), nullptr));
	HR(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, trianglepixelShader.GetAddressOf()));

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HR(device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), triangleinputLayout.GetAddressOf()));
}

void DX11Device::Render()
{
	float clearColor[4] = { 0.f, 0.f, 0.4f, 1.f };
	devcon->ClearRenderTargetView(rtv.Get(), clearColor);

	//Render Triangle
	devcon->IASetInputLayout(triangleinputLayout.Get());
	devcon->VSSetShader(trianglevertexShader.Get(), nullptr, 0);
	devcon->PSSetShader(trianglepixelShader.Get(), nullptr, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, triangleBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(3, 0);

	swapchain->Present(1, 0);
}
