#pragma once
#include "DX11Device.h"
#include "Camera.h"
#include "Model.h"

class Scene
{
public:
	Scene() = default;
	Scene(HWND hWnd);
	~Scene();

	void Render();
private:
	
	unique_ptr<DX11Device> device;
	unique_ptr<Camera> camera;
	Model* model = nullptr;

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyBoardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

public:

	LRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

