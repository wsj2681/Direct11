#pragma once
#include "Shader.h"

struct LightConstantBuffer
{
	XMFLOAT3 direction;
	float padding1;
	XMFLOAT3 color;
	float padding2;
	XMFLOAT3 ambientColor;
	float padding3;
	XMFLOAT3 cameraPosition;
	float padding4;
};

class LightShader : public Shader
{
public:
	LightShader();
	~LightShader() = default;

	void SetConstantBuffer(ComPtr<ID3D11Device>& device) override;
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMFLOAT3& cameraPosition);
	void Bind(ComPtr<ID3D11DeviceContext>& devcon) override;

	void SetLightDirection(const XMFLOAT3& dir);
	void SetLightColor(const XMFLOAT3& color);
	void SetLightAmbient(const XMFLOAT3& ambient);

private:


	ComPtr<ID3D11Buffer> LightconstantBuffer;
	XMFLOAT3 direction;
	XMFLOAT3 color;
	XMFLOAT3 ambient;
};

