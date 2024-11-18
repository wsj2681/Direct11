#include "Polygon.h"

CPolygon::CPolygon(ComPtr<ID3D11Device>& device, UINT vertexCount)
{
	worldMatrix = XMMatrixIdentity();
	worldMatrix = XMMatrixTranslation(0.f, 0.f, 0.f);
	rotation = { 0.f, 0.f, 0.f };
	CreatePolygonVertex(vertexCount);
	CreateVertexBuffer(device);
	CreateIndexBuffer(device);
	CreateConstantBuffer(device);
	CreateShader(device);
}



void CPolygon::CreatePolygonVertex(UINT vertexCount)
{
	const float radius = 0.5f;
	const float angleIncrement = XM_2PI / vertexCount;

	// Create ColorVertex
	vertices.resize(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		float angle = i * angleIncrement;
		vertices.at(i) = { {radius * sin(angle), radius * cos(angle), 0.f}, GenerateRandomColor() };
	}

	// Create Index
	for (UINT i = 1; i < vertexCount - 1; ++i)
	{
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	indexCount = indices.size();
}

void CPolygon::CreateVertexBuffer(ComPtr<ID3D11Device>& device)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(ColorVertex));
	bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = vertices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, vertexBuffer.GetAddressOf()));
}

void CPolygon::CreateIndexBuffer(ComPtr<ID3D11Device>& device)
{
	D3D11_BUFFER_DESC bufferdesc = {};
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));;
	bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata = {};
	initdata.pSysMem = indices.data();

	HR(device->CreateBuffer(&bufferdesc, &initdata, indexBuffer.GetAddressOf()));
}

void CPolygon::CreateConstantBuffer(ComPtr<ID3D11Device>& device)
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(ConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;

	HR(device->CreateBuffer(&cbDesc, nullptr, constantBuffer.GetAddressOf()));
}

void CPolygon::CreateShader(ComPtr<ID3D11Device>& device)
{
	// ColorVertex Shader
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

void CPolygon::Render(ComPtr<ID3D11DeviceContext>& devcon, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	worldMatrix = rotationMatrix * XMMatrixTranslation(0.f, 0.f, 0.f);
	rotation.y+=0.01f;

	ConstantBuffer cb = {};
	cb.world = XMMatrixTranspose(worldMatrix);
	cb.view = XMMatrixTranspose(viewMatrix);
	cb.projection = XMMatrixTranspose(projectionMatrix);

	devcon->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

	devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devcon->IASetInputLayout(inputLayout.Get());
	devcon->VSSetShader(vertexShader.Get(), nullptr, 0);
	devcon->PSSetShader(pixelShader.Get(), nullptr, 0);

	UINT stride = sizeof(ColorVertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(indexCount, 0, 0);
}
