#pragma once
class Camera
{
public:
	Camera() = default;
	Camera(const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& upVector);
	~Camera() = default;

	XMMATRIX& GetViewMatrix()
	{
		return this->viewMatrix;
	}
	XMMATRIX& GetProjectionMatrix()
	{
		return this->projectionMatrix;
	}

	void SetPosition(const XMFLOAT3& position)
	{
		this-> position = position;
		UpdateViewMatrix();
	}

	void UpdateCameraRotation(const float& deltaX, const float& deltaY);
	void UpdateViewMatrix();

private:

	XMFLOAT3 position;
	XMFLOAT3 target; // LookAt
	XMFLOAT3 upVector;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	float yaw = 0.f;
	float pitch = 0.f;
};

