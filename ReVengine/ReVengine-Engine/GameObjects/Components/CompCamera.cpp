#include "CompCamera.h"
#include "Rendering/Camera/Camera.h"
#include <Utils/MousePos.h>
#include <Rev_CoreSystems.h>
#include "GameObjects/Components/CompTransform.h"

using namespace Rev;

CompCamera::CompCamera(GameObject* gameObj, Rev::CompTransform* transform, bool flipControlls) :
	BaseComponent(gameObj),
	m_Camera{ std::make_unique<RevDev::Camera>() },
	m_Transform{ transform },
	m_LookSensitivity{5},
	m_ControllsFlipped{flipControlls},
	m_LockedX{false},
	m_LockedY{false}
{

}

void CompCamera::LateUpdate([[maybe_unused]] float deltaTime)
{
	m_Camera->Update(m_Transform->GetWorldPosition(), m_Transform->GetWorldRotation());

	MouseRelativeMotion delta = Rev::Rev_CoreSystems::pInputManager->GetMouseRelativeMotion();

	std::pair<float, float> turnValue = { (float)delta.x * m_LookSensitivity * deltaTime, (float)delta.y * m_LookSensitivity * deltaTime };

	if(m_ControllsFlipped)
		Turn(turnValue.first, turnValue.second);
	else
		Turn(turnValue.second, turnValue.first);
}

void CompCamera::Turn(float x, float y)
{
	if (m_LockedX) x = 0;
	if (m_LockedY) y = 0;
	m_Transform->Turn(x, y);
}

void CompCamera::LockAxes(RotationAxes axe, bool lock)
{
	if (axe == RotationAxes::x)
		m_LockedX = lock;
	else
		m_LockedY = lock;
}