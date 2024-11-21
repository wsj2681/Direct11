#include "framework.h"
#include "Scene.h"

Scene::Scene(HWND hWnd)
	:hWnd(hWnd)
{
	device = make_unique<DX11Device>(hWnd);
	camera = make_unique<Camera>(
		XMFLOAT3(0.0f, 0.0f, -3.0f), // Position
		XMFLOAT3(0.0f, 0.0f, 0.0f),  // LookAt
		XMFLOAT3(0.0f, 1.0f, 0.0f)   // UpVector
	);

	model = new Model(device.get()->GetDevice(),device.get()->GetDeviceContext(), 
		"Resource\\orkobj.obj", "Resource\\orkobj.mtl", "LightShader.hlsl", "LightShader.hlsl", "Resource\\orkobj.jpg");

	model->Move(-1.f, 0.f, 0.f);
}

Scene::~Scene()
{
	model->~Model();
	camera.release();
	device.release();
}

void Scene::Render()
{
	device->ClearBackBuffer();

	// Object Update
	if (model)
	{
		//model->Update();
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

		if (wParam & MK_MBUTTON) // 가운데 버튼 눌린 상태에서 움직임 감지
		{
			int deltaX = currentMousePosition.x - lastMousePosition.x;
			int deltaY = currentMousePosition.y - lastMousePosition.y;

			// 모델 회전 적용
			if (model && IsMouseOverModel(hWnd, currentMousePosition.x, currentMousePosition.y))
			{
				model->Rotate(deltaX * 0.005f, deltaY * 0.005f);
			}
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

bool Scene::IsMouseOverModel(HWND hWnd, int mouseX, int mouseY)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	int screenWidth = rect.right - rect.left;
	int screenHeight = rect.bottom - rect.top;

	// 뷰 행렬과 투영 행렬 가져오기
	XMMATRIX viewMatrix = camera->GetViewMatrix();
	XMMATRIX projectionMatrix = camera->GetProjectionMatrix();

	// 마우스 위치를 Normalized Device Coordinates로 변환
	float ndcX = (2.0f * mouseX / screenWidth - 1.0f);
	float ndcY = (1.0f - 2.0f * mouseY / screenHeight);

	// Ray의 시작점과 방향 계산
	XMVECTOR rayStart = XMVector3TransformCoord(
		XMVectorSet(ndcX, ndcY, 0.0f, 1.0f),
		XMMatrixInverse(nullptr, projectionMatrix * viewMatrix));
	XMVECTOR rayEnd = XMVector3TransformCoord(
		XMVectorSet(ndcX, ndcY, 1.0f, 1.0f),
		XMMatrixInverse(nullptr, projectionMatrix * viewMatrix));

	XMVECTOR rayDir = XMVector3Normalize(rayEnd - rayStart);

	// 모델의 경계와 충돌 확인
	return model->CheckRayIntersection(rayStart, rayDir);
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
