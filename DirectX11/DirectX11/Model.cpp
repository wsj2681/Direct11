#include "framework.h"
#include "Model.h"

Model::Model(ComPtr<ID3D11Device>& device, const vector<DiffusedVertex>& vertices, const vector<UINT>& indices, const wchar_t* vsPath, const wchar_t* psPath)
{
	diffusedMesh = new DiffusedMesh(device, vertices, indices);
	shader = new Shader(device, vsPath, psPath);
	
	shader->SetConstantBuffer(device);
}

Model::Model(ComPtr<ID3D11Device>& device, const vector<DiffusedVertex>& vertices, const vector<UINT>& indices, const wchar_t* vsPath, const wchar_t* psPath, const wchar_t* textureFile)
{
	diffusedMesh = new DiffusedMesh(device, vertices, indices);
	shader = new TextureShader(device, vsPath, psPath, textureFile);
	shader->SetConstantBuffer(device);
}

Model::Model(ComPtr<ID3D11Device>& device, const wchar_t* objFile, const wchar_t* vsPath, const wchar_t* psPath, const wchar_t* textureFile)
{
    vector<ModelVertex> vertices;
    vector<UINT> indices;
    LoadOBJ(objFile, vertices, indices);

    modelMesh = new ModelMesh(device, vertices, indices);
    shader = new TextureShader(device, vsPath, psPath, textureFile);
    shader->SetConstantBuffer(device);
}

Model::Model(ComPtr<ID3D11Device>& device, const string& fbxFile, const wchar_t* vsPath, const wchar_t* psPath, const wchar_t* textureFile)
{
    vector<ModelVertex> vertices;
    vector<UINT> indices;
    LoadFBX(fbxFile, vertices, indices);

    modelMesh = new ModelMesh(device, vertices, indices);
    shader = new TextureShader(device, vsPath, psPath, textureFile);
    shader->SetConstantBuffer(device);
}

void Model::Update()
{
	rotation.y += 0.01f;
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	worldMatrix = rotationMatrix * XMMatrixTranslation(0.f, 0.f, 0.f);
}

void Model::Render(ComPtr<ID3D11DeviceContext>& devcon, const XMMATRIX& view, const XMMATRIX& projection)
{
	shader->UpdateConstantBuffer(devcon, worldMatrix, view, projection);
	shader->Bind(devcon);

	if (diffusedMesh)
	{
		diffusedMesh->Render(devcon);
	}
	else if (modelMesh)
	{
		modelMesh->Render(devcon);
	}
}

struct VertexHasher 
{
    size_t operator()(const ModelVertex& v) const 
    {
        auto hash1 = hash<float>()(v.position.x) ^ hash<float>()(v.position.y) ^ hash<float>()(v.position.z);
        auto hash2 = hash<float>()(v.normal.x) ^ hash<float>()(v.normal.y) ^ hash<float>()(v.normal.z);
        auto hash3 = hash<float>()(v.uv.x) ^ hash<float>()(v.uv.y);
        return hash1 ^ hash2 ^ hash3;
    }
};

void Model::LoadOBJ(const wchar_t* objFile, vector<ModelVertex>& vertices, vector<UINT>& indices)
{
    vector<XMFLOAT3> tempPositions;
    vector<XMFLOAT3> tempNormals;
    vector<XMFLOAT2> tempUvs;

    unordered_map<ModelVertex, uint32_t, VertexHasher> uniqueVertices;

    ifstream file(objFile);
    if (!file.is_open()) 
    {
        wcerr << "Failed to open file: " << objFile << endl;
        return;
    }

    string line;
    while (getline(file, line)) 
    {
        istringstream ss(line);
        string prefix;
        ss >> prefix;

        if (prefix == "v") 
        {
            // Vertex position
            XMFLOAT3 position;
            ss >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        }
        else if (prefix == "vn") 
        {
            // Vertex normal
            XMFLOAT3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "vt") 
        {
            // Texture coordinate
            XMFLOAT2 texcoord;
            ss >> texcoord.x >> texcoord.y;
            tempUvs.push_back(texcoord);
        }
        else if (prefix == "f") 
        {
            // Face (polygon)
            string vertexData;
            for (int i = 0; i < 3; ++i) 
            {
                ss >> vertexData;

                istringstream vs(vertexData);
                string v, t, n;
                getline(vs, v, '/');
                getline(vs, t, '/');
                getline(vs, n, '/');

                int vIndex = stoi(v) - 1;
                int tIndex = t.empty() ? -1 : stoi(t) - 1;
                int nIndex = n.empty() ? -1 : stoi(n) - 1;

                ModelVertex vertex{};
                vertex.position = tempPositions[vIndex];
                if (nIndex >= 0) vertex.normal = tempNormals[nIndex];
                if (tIndex >= 0) vertex.uv = tempUvs[tIndex];

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    file.close();
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
}

void Model::ProcessMesh(FbxMesh* mesh, vector<ModelVertex>& vertices, vector<UINT>& indices)
{
    int vertexCount = mesh->GetControlPointsCount();
    FbxVector4* controlPoints = mesh->GetControlPoints();
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
                mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvSetNameList[0], fbxUV, unmapped);
                texcoord = XMFLOAT2(
                    static_cast<float>(fbxUV[0]),
                    static_cast<float>(fbxUV[1])
                );
            }

            // Create and store the vertex
            ModelVertex vertex = { position, normal, texcoord };
            vertices.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(vertices.size() - 1));
        }
    }
}
