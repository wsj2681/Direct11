#include "framework.h"
#include "Camera.h"

Camera::Camera()
{
	position = { 0.f, 0.f, 0.f };
	target = { 0.f, 0.f, 0.f };
	upVector = { 0.f, 0.f, 0.f };

	viewMatrix = XMMatrixLookAtLH(
		XMVectorSet(position.x, position.y, position.z, 1.0f),
		XMVectorSet(target.x, target.y, target.z, 1.0f),
		XMVectorSet(upVector.x, upVector.y, upVector.z, 0.0f)
	);

	projectionMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		ASPECTRATIO,
		0.1f,
		100.0f
	);
}

Camera::Camera(const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& upVector)
	:position(position), target(target), upVector(upVector)
{
	viewMatrix = XMMatrixLookAtLH(
		XMVectorSet(position.x, position.y, position.z, 1.0f),
		XMVectorSet(target.x, target.y, target.z, 1.0f),
		XMVectorSet(upVector.x, upVector.y, upVector.z, 0.0f)
	);

	projectionMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		ASPECTRATIO,
		0.1f,
		100.0f
	);
}

void Camera::UpdateCameraRotation(const float& deltaX, const float& deltaY)
{
	constexpr float sensitivity = 0.0005f;
	yaw += deltaX * sensitivity;
	pitch -= deltaY * sensitivity;

	constexpr float limit = XMConvertToRadians(89.0f);

	if (pitch > limit)
	{
		pitch = limit;
	}
	if (pitch < -limit)
	{
		pitch = -limit;
	}

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR forward = XMVector3Normalize(XMVectorSet(
		cosf(pitch) * sinf(yaw),
		sinf(pitch),
		cosf(pitch) * cosf(yaw),
		0.0f
	));

	XMVECTOR positionVec = XMVectorSet(position.x, position.y, position.z, 1.0f);
	XMVECTOR targetVec = XMVectorAdd(positionVec, forward);
	XMVECTOR upVec = XMVectorSet(upVector.x, upVector.y, upVector.z, 0.0f);

	viewMatrix = XMMatrixLookAtLH(positionVec, targetVec, upVec);
}
