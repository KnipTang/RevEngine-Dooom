#include "BulletComp.h"
#include "GameObjects/Components/CompTransform.h"
#include "GameObjects/GameObject.h"
#include <iostream>

BulletComp::BulletComp(Rev::GameObject* gameObj, float speed) :
	Rev::BaseComponent(gameObj),
	m_Transform(gameObj->transform),
	m_StartDirection(gameObj->transform->GetWorldRotation()),
	m_Speed(speed),
	m_DistanceTravelled{0},
	m_MaxTravelDistance{999}
{

}

BulletComp::~BulletComp()
{
}

void BulletComp::fixedUpdate([[maybe_unused]] float fixedDeltaTime)
{
	m_Transform->MoveForward(1, m_Speed * fixedDeltaTime);
	m_DistanceTravelled += m_Speed * fixedDeltaTime;
	if (m_DistanceTravelled > m_MaxTravelDistance)
		m_GameObject->Destroy();
}