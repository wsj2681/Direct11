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

	model->Move(0.f, 0.f, 0.f);

	// ImGui �ʱ�ȭ
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.DisplaySize = ImVec2(static_cast<float>(FRAMEBUFFER_WIDTH), static_cast<float>(FRAMEBUFFER_HEIGHT));
	float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(hWnd);
	io.DisplayFramebufferScale = ImVec2(dpiScale, dpiScale);

	// ImGui �鿣�� ����
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device->GetDevice().Get(), device->GetDeviceContext().Get());
	ImGui::StyleColorsDark(); // ��ũ �׸� ����

	ImGui_ImplWin32_EnableDpiAwareness();
}

Scene::~Scene()
{
	model->~Model();
	camera.release();
	device.release();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Scene::Render()
{
	static float sliderValueR = 1.0f;
	static float sliderValueG = 1.0f;
	static float sliderValueB = 1.0f;
	device->ClearBackBuffer();

	// Object Update
	if (model)
	{
		//model->Update();
	}

	// Object Draw
	if (model)
	{
		model->Render(device->GetDeviceContext(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), 1.0f);
	}


	// ImGui ������ ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// UI ��� �׸���
	ImGui::Begin("Example UI");
	ImGui::Text("Hello, ImGui!");
	ImGui::SliderFloat("SliderR", &sliderValueR, 0.1f, 1.0f);
	ImGui::SliderFloat("SliderG", &sliderValueG, 0.1f, 1.0f);
	ImGui::SliderFloat("SliderB", &sliderValueB, 0.1f, 1.0f);
	if (ImGui::Button("Click Me")) 
	{
		std::cout << sliderValueR << std::endl;
		std::cout << sliderValueG << std::endl;
		std::cout << sliderValueB << std::endl;
	}
	ImGui::End();

	// ImGui ������
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	model->lightShader->SetLightAmbient({ sliderValueR , sliderValueG ,sliderValueB });

	device->Render();
}

void Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static POINT lastMousePosition = {};
	POINT currentMousePosition;
	switch (nMessageID) 
	{
	case WM_LBUTTONDOWN:
	{
		ImGuiIO& io = ImGui::GetIO();
		float mouseX = io.MousePos.x; // ImGui�� ���콺 X ��ǥ
		float mouseY = io.MousePos.y; // ImGui�� ���콺 Y ��ǥ

		// ���÷��� ũ��� ��
		std::cout << "Mouse Position: (" << mouseX << ", " << mouseY << ")\n";
		std::cout << "Display Size: (" << io.DisplaySize.x << ", " << io.DisplaySize.y << ")\n";

		GetCursorPos(&currentMousePosition);
		ScreenToClient(hWnd, &currentMousePosition);
		std::cout << "Mouse Position: (" << currentMousePosition.x << ", " << currentMousePosition.y << ")\n";
		std::cout << "Display Size: (" << io.DisplaySize.x << ", " << io.DisplaySize.y << ")\n";
		break;
	}
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
	{
		GetCursorPos(&currentMousePosition);
		ScreenToClient(hWnd, &currentMousePosition);

		if (wParam & MK_MBUTTON) // ��� ��ư ���� ���¿��� ������ ����
		{
			int deltaX = currentMousePosition.x - lastMousePosition.x;
			int deltaY = currentMousePosition.y - lastMousePosition.y;

			// �� ȸ�� ����
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

	// �� ��İ� ���� ��� ��������
	XMMATRIX viewMatrix = camera->GetViewMatrix();
	XMMATRIX projectionMatrix = camera->GetProjectionMatrix();

	// ���콺 ��ġ�� Normalized Device Coordinates�� ��ȯ
	float ndcX = (2.0f * mouseX / screenWidth - 1.0f);
	float ndcY = (1.0f - 2.0f * mouseY / screenHeight);

	// Ray�� �������� ���� ���
	XMVECTOR rayStart = XMVector3TransformCoord(
		XMVectorSet(ndcX, ndcY, 0.0f, 1.0f),
		XMMatrixInverse(nullptr, projectionMatrix * viewMatrix));
	XMVECTOR rayEnd = XMVector3TransformCoord(
		XMVectorSet(ndcX, ndcY, 1.0f, 1.0f),
		XMMatrixInverse(nullptr, projectionMatrix * viewMatrix));

	XMVECTOR rayDir = XMVector3Normalize(rayEnd - rayStart);

	// ���� ���� �浹 Ȯ��
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
