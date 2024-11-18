#include "framework.h"
#include "Camera.h"

Camera::Camera(const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& upVector)
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
