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

	model = new Model(device.get()->GetDevice(), "FinalBaseMesh.obj", "textureShader.hlsl", "textureShader.hlsl", "sample.dds");
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

void Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static POINT lastMousePosition = {};
	POINT currentMousePosition;
	switch (nMessageID) 
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
	{
		GetCursorPos(&currentMousePosition);
		ScreenToClient(hWnd, &currentMousePosition);

		int deltaX = currentMousePosition.x - lastMousePosition.x;
		int deltaY = currentMousePosition.y - lastMousePosition.y;

		if (wParam & MK_LBUTTON)
		{
			camera->Rotate(deltaX * 0.005f, deltaY * 0.005f);
		}
		lastMousePosition = currentMousePosition;
	}
		break;
	default: 
		break;
	}
}

void Scene::OnProcessingKeyBoardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID) 
	{
	case WM_KEYUP:
		switch (wParam) 
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_F8:
			break;
		case VK_F9:
			break;
		default: 
			break;
		}
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'W':
		{
			camera->Move({ 0.0f, 0.0f, 1.0f }, 0.016f);
			break;
		}
		case 'S':
		{
			camera->Move({ 0.0f, 0.0f, -1.0f }, 0.016f);
			break;
		}
		case 'A':
		{
			camera->Move({ -1.0f, 0.0f, 0.0f }, 0.016f);
			break;
		}
		case 'D':
		{
			camera->Move({ 1.0f, 0.0f, 0.0f }, 0.016f);
			break;
		}
		default:
			break;
		}
	}
	default: 
		break;
	}
}

LRESULT Scene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyBoardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	default:
		break;
	}

	return 0;
}
