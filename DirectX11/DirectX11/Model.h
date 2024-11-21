#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "TextureShader.h"
#include "LightShader.h"

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

    // .fbx 葛胆 积己
    Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon, 
        const string& fbxFile, const string& vsPath, const string& psPath, const string& textureFile);

    // .obj 葛胆 积己
    Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon,
        const string& objFile, const string& mtlFile, const string& vsPath, const string& psPath, const string& textureFile);

    ~Model();

    void Update();
    void Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection);

    void Rotate(float deltaX, float deltaY);
    void Move(float deltaX, float deltaY, float deltaZ);

    bool CheckRayIntersection(const XMVECTOR& rayOrigin, const XMVECTOR& rayDirection);

private:

    void LoadOBJ(const string& objFile, const string& mtlBasePath, vector<ModelVertex>& vertices, vector<UINT>& indices, string& textureFile);
    void LoadFBX(const string& fbxFile, vector<ModelVertex>& vertices, vector<UINT>& indices);
    void ProcessMesh(FbxMesh* mesh, vector<ModelVertex>& vertices, vector<UINT>& indices);

    Mesh<ModelVertex>* modelMesh = nullptr;
    Shader* shader = nullptr;
    LightShader* lightShader = nullptr;
    XMMATRIX worldMatrix = XMMatrixIdentity();
    XMFLOAT3 rotation = { 0.f, 0.f, 0.f };
    XMFLOAT3 position = { 0.f, 0.f, 0.f };
    BoundingBox boundingBox;
};

