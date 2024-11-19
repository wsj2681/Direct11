#include "framework.h"
#include "Scene.h"

Scene::Scene(HWND hWnd)
{
	device = make_unique<DX11Device>(hWnd);
	camera = make_unique<Camera>(
		XMFLOAT3(0.0f, 2.0f, -20.0f), // Position
		XMFLOAT3(0.0f, 0.0f, 0.0f),  // LookAt
		XMFLOAT3(0.0f, 1.0f, 0.0f)   // UpVector
	);

	UINT vertexCount = 5;
	const float radius = 0.5f;
	const float angleIncrement = XM_2PI / vertexCount;

	vector<DiffusedVertex> vertices(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		float angle = i * angleIncrement;
		vertices.at(i) = { {radius * sin(angle), radius * cos(angle), 0.f}, GenerateRandomColor() };
	}

	vector<UINT> indices;
	for (UINT i = 1; i < vertexCount - 1; ++i)
	{
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	
	vector<DiffusedVertex> cubevertices = {
	{{-1.0f, +1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 0
	{{+1.0f, +1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 1
	{{+1.0f, +1.0f, +1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // 2
	{{-1.0f, +1.0f, +1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // 3
	{{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // 4
	{{+1.0f, -1.0f, -1.0f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 5
	{{+1.0f, -1.0f, +1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // 6
	{{-1.0f, -1.0f, +1.0f}, {1.0f, 0.5f, 0.0f, 1.0f}}, // 7
	};

	vector<UINT> cubeindices = {
		// Front face edges
		3,1,0,
		2,1,3,
		0,5,4,
		1,5,0,
		3,4,7,
		0,4,3,
		1,6,5,
		2,6,1,
		2,7,6,
		3,7,2,
		6,4,5,
		7,4,6
	};

	//model = new Model(device.get()->GetDevice(), vertices, indices, L"textureShader.hlsl", L"textureShader.hlsl", L"sample.dds");
	//model = new Model(device.get()->GetDevice(), cubevertices, cubeindices, L"triangleShader.hlsl", L"triangleShader.hlsl");

	model = new Model(device.get()->GetDevice(), L"FinalBaseMesh.obj", L"textureShader.hlsl", L"textureShader.hlsl", L"sample.dds");
}

void Scene::Render()
{
	device->ClearBackBuffer();

	// Object Update
	if (model)
	{
		model->Update();
	}

	// Object Draw
	if (model)
	{
		model->Render(device->GetDeviceContext(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

	}
	device->Render();
}
