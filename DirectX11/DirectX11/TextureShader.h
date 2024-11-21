#pragma once
#include "Shader.h"
class TextureShader : public Shader
{
public:
	TextureShader() = default;
	TextureShader(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon, const string& vsPath, const string& psPath, const string& textureFile);
	~TextureShader();

	void SetConstantBuffer(ComPtr<ID3D11Device>& device) override;
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) override;
	void Bind(ComPtr<ID3D11DeviceContext>& devcon) override;

private:

	ComPtr<ID3D11ShaderResourceView> textureView;
	ComPtr<ID3D11SamplerState> samplerstate;

};

