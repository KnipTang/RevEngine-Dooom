#include "LookAtPlayerComp.h"
#include "GameObjects/Components/CompTransform.h"
#include "GameObjects/GameObject.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>


LookAtPlayerComp::LookAtPlayerComp(Rev::GameObject* gameObj, Rev::CompTransform* playerTransform) :
	Rev::BaseComponent(gameObj),
	m_PlayerTransform{ playerTransform },
	m_ObjectTransform{ m_GameObject->transform }
{

}

LookAtPlayerComp::~LookAtPlayerComp()
{
}

void LookAtPlayerComp::Update([[maybe_unused]] float deltaTime)
{
	//if (m_PlayerTransform->IsPositionDirty())
		TurnToPlayer();
}

void LookAtPlayerComp::TurnToPlayer()
{
	glm::vec2 player2DPos = { m_PlayerTransform->GetWorldPosition().x , m_PlayerTransform->GetWorldPosition().z };
	glm::vec2 enemy2DPos = { m_ObjectTransform->GetWorldPosition().x , m_ObjectTransform->GetWorldPosition().z };

    glm::vec2 vector = enemy2DPos - player2DPos;

    glm::vec2 reference(1.0f, 0.0f);

    float angle = glm::orientedAngle(glm::normalize(reference), glm::normalize(vector));
    angle -= 1.5708;
    angle = -angle;

	m_ObjectTransform->SetRotationRad(m_ObjectTransform->GetWorldRotation().x, angle, m_ObjectTransform->GetWorldRotation().z);
}