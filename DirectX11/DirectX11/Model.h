#pragma once
#include "Mesh.h"
#include "Shader.h"

class Model
{
public:

    Model() = default;
    Model(ComPtr<ID3D11Device>& device, const vector<Vertex>& vertices, const vector<UINT>& indices, const wchar_t* vsPath, const wchar_t* psPath);
    ~Model() = default;

    void Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection);

private:

    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
    XMMATRIX worldMatrix = XMMatrixIdentity();
};

