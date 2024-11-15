#include "framework.h"
#include "Polygon.h"

CPolygon::CPolygon(ComPtr<ID3D11Device>& device, UINT vertexCount)
{
	const float radius = 0.5f;
	const float angleIncrement = XM_2PI / vertexCount;

	// Create Vertex
	vector<Vertex> vertices(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		float angle = i * angleIncrement;
		vertices.at(i) = { {radius * sin(angle), radius * cos(angle), 0.f}, {1.0f, 0.0f, 0.0f, 1.0f} };
	}

	// Create Index
	vector<UINT> indices;
	for (UINT i = 1; i < vertexCount - 1; ++i)
	{
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	indices.push_back(0);
	indices.push_back(vertexCount - 1);
	indices.push_back(1);
	indexCount = indices.size();

	// Vertex Buffer
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));
	bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = vertices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, vertexBuffer.GetAddressOf()));

	// Index Buffer
	bufferdesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));;
	bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	initdata.pSysMem = indices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, indexBuffer.GetAddressOf()));

	// Vertex Shader
	ComPtr<ID3DBlob> vsBlob;
	HR(D3DCompileFromFile(L"triangleShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, vsBlob.GetAddressOf(), nullptr));
	HR(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// Pixel Shader
	ComPtr<ID3DBlob> psBlob;
	HR(D3DCompileFromFile(L"triangleShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, psBlob.GetAddressOf(), nullptr));
	HR(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HR(device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), inputLayout.GetAddressOf()));
}

void CPolygon::Render(ComPtr<ID3D11DeviceContext>& devcon)
{
	devcon->IASetInputLayout(inputLayout.Get());
	devcon->VSSetShader(vertexShader.Get(), nullptr, 0);
	devcon->PSSetShader(pixelShader.Get(), nullptr, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(indexCount, 0, 0);
}
