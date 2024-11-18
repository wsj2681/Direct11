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

private:

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

public:

};

