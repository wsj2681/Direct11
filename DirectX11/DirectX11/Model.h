#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "TextureShader.h"

class Model
{
public:

    Model() = default;
    Model(ComPtr<ID3D11Device>& device, const 
        vector<DiffusedVertex>& vertices, const vector<UINT>& indices, 
        const wchar_t* vsPath, const wchar_t* psPath);

    Model(ComPtr<ID3D11Device>& device, 
        const vector<DiffusedVertex>& vertices, const vector<UINT>& indices, 
        const wchar_t* vsPath, const wchar_t* psPath, const wchar_t* textureFile);

    Model(ComPtr<ID3D11Device>& device,
        const wchar_t* objFile,
        const wchar_t* vsPath, const wchar_t* psPath, const wchar_t* textureFile);

    Model(ComPtr<ID3D11Device>& device,
        const string& fbxFile,
        const wchar_t* vsPath, const wchar_t* psPath, const wchar_t* textureFile);

    ~Model() = default;

    void Update();
    void Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection);

private:

    void LoadOBJ(const wchar_t* objFile, vector<ModelVertex>& vertices, vector<UINT>& indices);
    void LoadFBX(const string& objFile, vector<ModelVertex>& vertices, vector<UINT>& indices);
    void ProcessMesh(FbxMesh* mesh, vector<ModelVertex>& vertices, vector<UINT>& indices);

    DiffusedMesh* diffusedMesh = nullptr;
    ModelMesh* modelMesh = nullptr;
    Shader* shader = nullptr;
    XMMATRIX worldMatrix = XMMatrixIdentity();
    XMFLOAT3 rotation = { 0.f, 0.f, 0.f };

};

