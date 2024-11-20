#pragma once
#include "framework.h"

class Shader
{
public:
    Shader() = default;
    Shader(ComPtr<ID3D11Device>& device, const string& vsPath, const string& psPath);
    ~Shader();

    virtual void SetConstantBuffer(ComPtr<ID3D11Device>& device);
    virtual void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection);
    virtual void Bind(ComPtr<ID3D11DeviceContext>& devcon);

protected:
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> inputLayout;
    ComPtr<ID3D11Buffer> constantBuffer;

    struct ConstantBuffer
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };
};