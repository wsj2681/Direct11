#include "LightShader.h"

LightShader::LightShader()
{
	direction = { 0.f, -1.0f, 0.f };
	color = { 1.0f, 1.0f, 1.0f };
	ambient = { 0.0f, 0.0f, 0.0f };
}

void LightShader::SetConstantBuffer(ComPtr<ID3D11Device>& device)
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(LightConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;

	HR(device->CreateBuffer(&cbDesc, nullptr, LightconstantBuffer.GetAddressOf()));
}

void LightShader::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMFLOAT3& cameraPosition, const float& shinness)
{
	LightConstantBuffer cb = {};
	cb.direction = direction;
	cb.color = color;
	cb.ambientColor = ambient;
	cb.cameraPosition = cameraPosition;
	cb.shinness = 1.0f;

	devcon->UpdateSubresource(LightconstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
}

void LightShader::Bind(ComPtr<ID3D11DeviceContext>& devcon)
{
	devcon->PSSetConstantBuffers(1, 1, LightconstantBuffer.GetAddressOf());
}

void LightShader::SetLightDirection(const XMFLOAT3& dir)
{
	this->direction = dir;
}

void LightShader::SetLightColor(const XMFLOAT3& color)
{
	this->color = color;
}

void LightShader::SetLightAmbient(const XMFLOAT3& ambient)
{
	this->ambient = ambient;
}