#pragma once
#include "Shader.h"
class TextureShader : public Shader
{
public:
	TextureShader() = default;
	TextureShader(ComPtr<ID3D11Device>& device, const wchar_t* vsPath, const wchar_t* psPath, const wchar_t* textureFile);
	~TextureShader();

	void SetConstantBuffer(ComPtr<ID3D11Device>& device) override;
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) override;
	void Bind(ComPtr<ID3D11DeviceContext>& devcon) override;

private:
	void LoadTexture(ComPtr<ID3D11Device>& device, const wchar_t* textureFile);

	ComPtr<ID3D11ShaderResourceView> textureView;
	ComPtr<ID3D11SamplerState> samplerstate;

};

