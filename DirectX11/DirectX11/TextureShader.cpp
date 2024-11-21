#include "WICTextureLoader11.h"
#include "DDSTextureLoader11.h"
#include "TextureShader.h"

TextureShader::TextureShader(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon, const string& vsPath, const string& psPath, const string& textureFile)
{
	// DiffusedVertex Shader
	ComPtr<ID3DBlob> vsBlob;
	HR(D3DCompileFromFile(TOWSTRING(vsPath), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, vsBlob.GetAddressOf(), nullptr));
	HR(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// Pixel Shader
	ComPtr<ID3DBlob> psBlob;
	HR(D3DCompileFromFile(TOWSTRING(psPath), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, psBlob.GetAddressOf(), nullptr));
	HR(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));

	//LoadTexture(device, textureFile);
	if (textureFile.back() == 'g' || textureFile.back() == 'G')
	{
		CreateWICTextureFromFile(device.Get(), TOWSTRING(textureFile), nullptr, textureView.GetAddressOf());
	}
	else if (textureFile.back() == 's' || textureFile.back() == 'S')
	{
		CreateDDSTextureFromFile(device.Get(), TOWSTRING(textureFile), nullptr, textureView.GetAddressOf());
	}
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HR(device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), inputLayout.GetAddressOf()));


	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HR(device->CreateSamplerState(&samplerDesc, samplerstate.GetAddressOf()));
	devcon->PSSetSamplers(0, 1, samplerstate.GetAddressOf());
}

TextureShader::~TextureShader()
{
	SAFE_RESET(textureView);
	SAFE_RESET(samplerstate);
}

void TextureShader::SetConstantBuffer(ComPtr<ID3D11Device>& device)
{
	Shader::SetConstantBuffer(device);
}

void TextureShader::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection)
{
	Shader::UpdateConstantBuffer(devcon, world, view, projection);
}

void TextureShader::Bind(ComPtr<ID3D11DeviceContext>& devcon)
{
	Shader::Bind(devcon);
	devcon->PSSetShaderResources(0, 1, textureView.GetAddressOf());
	devcon->PSSetSamplers(0, 1, samplerstate.GetAddressOf());
}