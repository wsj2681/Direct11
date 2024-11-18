#include "framework.h"
#include "Scene.h"

Scene::Scene(HWND hWnd)
{
	device = make_unique<DX11Device>(hWnd);
	camera = make_unique<Camera>(
		XMFLOAT3(0.0f, 0.0f, -5.0f), // Position
		XMFLOAT3(0.0f, 0.0f, 0.0f),  // LookAt
		XMFLOAT3(0.0f, 1.0f, 0.0f)   // UpVector
	);

	UINT vertexCount = 100;
	const float radius = 0.5f;
	const float angleIncrement = XM_2PI / vertexCount;

	vector<Vertex> vertices(vertexCount);
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

	model = new Model(device.get()->GetDevice(), vertices, indices, L"triangleShader.hlsl", L"triangleShader.hlsl");
}

void Scene::Render()
{
	device->ClearBackBuffer();

	if (model)
	{
		model->Render(device->GetDeviceContext(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

	}
	device->Render();
}
