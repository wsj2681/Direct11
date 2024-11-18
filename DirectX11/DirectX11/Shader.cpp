#include "Shader.h"

Shader::Shader(ComPtr<ID3D11Device>& device, const wchar_t* vsPath, const wchar_t* psPath)
{
	// Vertex Shader
	ComPtr<ID3DBlob> vsBlob;
	HR(D3DCompileFromFile(vsPath, nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, vsBlob.GetAddressOf(), nullptr));
	HR(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// Pixel Shader
	ComPtr<ID3DBlob> psBlob;
	HR(D3DCompileFromFile(psPath, nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, psBlob.GetAddressOf(), nullptr));
	HR(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HR(device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), inputLayout.GetAddressOf()));
}

void Shader::SetConstantBuffer(ComPtr<ID3D11Device>& device)
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(ConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;

	HR(device->CreateBuffer(&cbDesc, nullptr, constantBuffer.GetAddressOf()));
}

void Shader::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection)
{
	ConstantBuffer cb = {};
	cb.world = XMMatrixTranspose(world);
	cb.view = XMMatrixTranspose(view);
	cb.projection = XMMatrixTranspose(projection);

	devcon->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);
}

void Shader::Bind(ComPtr<ID3D11DeviceContext>& devcon)
{
	devcon->VSSetShader(vertexShader.Get(), nullptr, 0);
	devcon->PSSetShader(pixelShader.Get(), nullptr, 0);
	devcon->IASetInputLayout(inputLayout.Get());
	devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}
