#pragma once
#include "framework.h"

class Shader
{
public:
    Shader() = default;
    Shader(ComPtr<ID3D11Device>& device, const wchar_t* vsPath, const wchar_t* psPath, bool isTextured = false);
    ~Shader() = default;

    void SetConstantBuffer(ComPtr<ID3D11Device>& device);
    void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection);
    void Bind(ComPtr<ID3D11DeviceContext>& devcon);

private:
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> inputLayout;
    ComPtr<ID3D11Buffer> constantBuffer;

    ComPtr<ID3D11ShaderResourceView> textureView;
    ComPtr<ID3D11SamplerState> samplerstate;

    struct ConstantBuffer
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };
};

