#pragma once
#include "DX11Device.h"
#include "Camera.h"
#include "Model.h"

class Scene
{
public:
	Scene() = default;
	Scene(HWND hWnd);
	~Scene() = default;

private:
	
	unique_ptr<DX11Device> device;

	unique_ptr<Camera> camera;

	Model* model = nullptr;

public:

	void Render();
};

