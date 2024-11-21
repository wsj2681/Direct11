#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "TextureShader.h"

#pragma warning(push)
#pragma warning(disable : 26495)
#pragma warning(disable : 26813)
#include <fbxsdk.h>

#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libxml2.lib")

#pragma warning(pop)

class Model
{
public:

    Model() = default;
    Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon, 
        const string& objFile, const string& vsPath, const string& psPath, const string& textureFile);

    ~Model();

    void Update();
    void Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection);

private:

    void LoadOBJ(const string& objFile, const string& mtlBasePath, vector<ModelVertex>& vertices, vector<UINT>& indices, string& textureFile);
    void LoadFBX(const string& fbxFile, vector<ModelVertex>& vertices, vector<UINT>& indices);
    void ProcessMesh(FbxMesh* mesh, vector<ModelVertex>& vertices, vector<UINT>& indices);

    Mesh<ModelVertex>* modelMesh = nullptr;
    Shader* shader = nullptr;
    XMMATRIX worldMatrix = XMMatrixIdentity();
    XMFLOAT3 rotation = { 0.f, 0.f, 0.f };

};

