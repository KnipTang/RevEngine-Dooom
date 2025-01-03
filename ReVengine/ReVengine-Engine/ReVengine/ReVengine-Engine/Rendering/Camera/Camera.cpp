#include "Camera.h"
#include "Utils/ReV_Mat.h"

using namespace RevDev;

Camera::Camera()
{
}

Camera::~Camera()
{

}

void Camera::Update(glm::vec3 position, glm::vec3 rotation)
{
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0, 0.0, 1.0f);
	DirectX::XMVECTOR vecPosition = DirectX::XMVECTOR{ position.x, position.y, position.z };
	DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0, 0.0, 1.0f, 1.0f);

	float pitch = rotation.x;
	float yaw = rotation.y;
	float roll = rotation.z;

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	lookAt = DirectX::XMVectorAdd(vecPosition, lookAt);

	m_ViewMatrix = DirectX::XMMatrixLookAtLH(vecPosition, lookAt, up);
}
