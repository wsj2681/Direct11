#include "framework.h"
#include "tiny_obj_loader.h"
#include "Model.h"

Model::Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon, const string& objFile, const string& vsPath, const string& psPath, const string& textureFile)
{
    vector<ModelVertex> vertices;
    vector<UINT> indices;
    
    LoadFBX(objFile, vertices, indices);
    rotation.x = -90.f;

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    worldMatrix = rotationMatrix * XMMatrixTranslation(0.f, 0.f, 0.f);
    modelMesh = new Mesh<ModelVertex>(device, vertices, indices);
    shader = new TextureShader(device, devcon, vsPath, psPath, textureFile);
    shader->SetConstantBuffer(device);
    lightShader = new LightShader();
    lightShader->SetConstantBuffer(device);
}

Model::Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& devcon, const string& objFile, const string& mtlFile, const string& vsPath, const string& psPath, const string& textureFile)
{
    vector<ModelVertex> vertices;
    vector<UINT> indices;

    string path = textureFile;
    LoadOBJ(objFile, "Resource\\orkobj.mtl", vertices, indices, path);
    rotation.y = 90.f;

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    worldMatrix = rotationMatrix * XMMatrixTranslation(0.f, 0.f, 0.f);
    modelMesh = new Mesh<ModelVertex>(device, vertices, indices);
    shader = new TextureShader(device, devcon, vsPath, psPath, textureFile);
    shader->SetConstantBuffer(device);
    lightShader = new LightShader();
    lightShader->SetConstantBuffer(device);
}

Model::~Model()
{
    modelMesh->~Mesh();
    shader->~Shader();
}

void Model::Update()
{
	rotation.y += 0.005f;
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	worldMatrix = rotationMatrix * XMMatrixTranslation(0.f, 0.f, 0.f);
}

void Model::Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection)
{
	shader->UpdateConstantBuffer(devcon, worldMatrix, view, projection);
    lightShader->UpdateConstantBuffer(devcon, { 0.0f, 0.0f, -3.0f });
	shader->Bind(devcon);
    lightShader->Bind(devcon);
    modelMesh->Render(devcon);
}

void Model::LoadOBJ(const string& objFile, const string& mtlBasePath, vector<ModelVertex>& vertices, vector<UINT>& indices, string& textureFile)
{
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFile.c_str(), mtlBasePath.c_str())) 
    {
        cerr << "Failed to load OBJ file: " << warn << err << endl;
        return;
    }

    // Process materials
    if (!materials.empty()) 
    {
        textureFile = materials[0].diffuse_texname; // Assuming single material
    }

    // Process shapes
    for (const auto& shape : shapes) 
    {
        size_t indexOffset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) 
        {
            size_t fv = shape.mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; ++v) 
            {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

                // Positions
                XMFLOAT3 position(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]);

                // Normals
                XMFLOAT3 normal(0, 0, 0);
                if (idx.normal_index >= 0) 
                {
                    normal = XMFLOAT3(
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]);
                }

                // Texture coordinates
                XMFLOAT2 texcoord(0, 0);
                if (idx.texcoord_index >= 0) 
                {
                    texcoord = XMFLOAT2(
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        attrib.texcoords[2 * idx.texcoord_index + 1]);
                }

                // Add vertex
                vertices.push_back({ position, normal, texcoord });
                indices.push_back(static_cast<uint32_t>(indices.size()));
            }
            indexOffset += fv;
        }
    }

    return;
}



void Model::LoadFBX(const string& fbxFile, vector<ModelVertex>& vertices, vector<UINT>& indices)
{
    FbxManager* manager = FbxManager::Create();
    if (!manager)
    {
        cerr << "Failed to create FBX Manager!" << endl;
        return;
    }

    FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ioSettings);

    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(fbxFile.c_str(), -1, manager->GetIOSettings()))
    {
        manager->Destroy();
        return;
    }


    FbxScene* scene = FbxScene::Create(manager, "Scene");
    if (!importer->Import(scene))
    {
        importer->Destroy();
        manager->Destroy();
        return;
    }

    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    FbxAxisSystem directXAxisSystem(FbxAxisSystem::eDirectX);
    directXAxisSystem.ConvertScene(scene);

    FbxNode* rootNode = scene->GetRootNode();
    if (rootNode)
    {
        for (int i = 0; i < rootNode->GetChildCount(); ++i)
        {
            FbxNode* childNode = rootNode->GetChild(i);
            FbxMesh* mesh = childNode->GetMesh();
            if (mesh)
            {
                ProcessMesh(mesh, vertices, indices);
            }
        }
    }

    importer->Destroy();
    manager->Destroy();
}


struct VertexHash 
{
    size_t operator()(const ModelVertex& vertex) const 
    {
        return hash<float>()(vertex.position.x) ^
            hash<float>()(vertex.position.y) ^
            hash<float>()(vertex.position.z) ^
            hash<float>()(vertex.uv.x) ^
            hash<float>()(vertex.uv.y);
    }
    bool operator()(const ModelVertex& v1, const ModelVertex& v2) const 
    {
        return v1.position.x == v2.position.x &&
            v1.position.y == v2.position.y &&
            v1.position.z == v2.position.z &&
            v1.uv.x == v2.uv.x &&
            v1.uv.y == v2.uv.y;
    }
};

void Model::ProcessMesh(FbxMesh* mesh, vector<ModelVertex>& vertices, vector<UINT>& indices)
{
    int vertexCount = mesh->GetControlPointsCount();
    FbxVector4* controlPoints = mesh->GetControlPoints();

    unordered_map<ModelVertex, uint32_t, VertexHash, VertexHash> uniqueVertices;

    vertices.reserve(vertexCount);

    int polygonCount = mesh->GetPolygonCount();
    for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex) 
    {
        for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex) 
        { // Assumes triangulated mesh
            int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);

            // Extract position
            FbxVector4 fbxPosition = controlPoints[controlPointIndex];
            XMFLOAT3 position(
                static_cast<float>(fbxPosition[0]),
                static_cast<float>(fbxPosition[1]),
                static_cast<float>(fbxPosition[2])
            );

            // Extract normal
            XMFLOAT3 normal(0, 0, 0);
            FbxVector4 fbxNormal;
            if (mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, fbxNormal)) 
            {
                normal = XMFLOAT3(
                    static_cast<float>(fbxNormal[0]),
                    static_cast<float>(fbxNormal[1]),
                    static_cast<float>(fbxNormal[2])
                );
            }

            // Extract texture coordinates
            XMFLOAT2 texcoord(0, 0);
            FbxStringList uvSetNameList;
            mesh->GetUVSetNames(uvSetNameList);
            if (uvSetNameList.GetCount() > 0) 
            {
                FbxVector2 fbxUV;
                bool unmapped;
                if (mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvSetNameList[0], fbxUV, unmapped))
                {
                    texcoord = XMFLOAT2(
                        static_cast<float>(fbxUV[0]),
                        static_cast<float>(fbxUV[1])
                    );
                }
            }

            // Create and store the vertex
            ModelVertex vertex = { position, normal, texcoord };
            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}
