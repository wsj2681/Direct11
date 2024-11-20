#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "TextureShader.h"

class Model
{
public:

    Model() = default;
    // 일반 도형 생성
    Model(ComPtr<ID3D11Device>& device, 
        const vector<DiffusedVertex>& vertices, const vector<UINT>& indices, const string& vsPath, const string& psPath);

    // 
    Model(ComPtr<ID3D11Device>& device, 
        const vector<DiffusedVertex>& vertices, const vector<UINT>& indices, const string& vsPath, const string& psPath, const string& textureFile);

    Model(ComPtr<ID3D11Device>& device, 
        const string& objFile, const string& vsPath, const string& psPath, const string& textureFile);

    ~Model() = default;

    void Update();
    void Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection);

private:

    void LoadOBJ(const string& objFile, vector<ModelVertex>& vertices, vector<UINT>& indices);
    void LoadFBX(const string& fbxFile, vector<ModelVertex>& vertices, vector<UINT>& indices);
    void ProcessMesh(FbxMesh* mesh, vector<ModelVertex>& vertices, vector<UINT>& indices);

    DiffusedMesh* diffusedMesh = nullptr;
    ModelMesh* modelMesh = nullptr;
    Shader* shader = nullptr;
    XMMATRIX worldMatrix = XMMatrixIdentity();
    XMFLOAT3 rotation = { 0.f, 0.f, 0.f };

};

