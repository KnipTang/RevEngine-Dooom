#include "EnemyComp.h"
#include "GameObjects/Components/CompTransform.h"
#include "GameObjects/GameObject.h"
#include <Rev_CoreSystems.h>

EnemyComp::EnemyComp(Rev::GameObject* gameObj) :
	Rev::BaseComponent(gameObj),
	m_EnemyTransform{ m_GameObject->transform },
	m_TimeLastPlayedSound{0},
	m_TimePlayedSound{10},
	m_Speed{1}
{

}

EnemyComp::~EnemyComp()
{
}

void EnemyComp::FixedUpdate([[maybe_unused]] float fixedDeltaTime)
{
	WalkToPlayer(fixedDeltaTime);
	PlayEnemySound(fixedDeltaTime);
}

void EnemyComp::WalkToPlayer(float fixedDeltaTime)
{	
	m_EnemyTransform->MoveForward(-1, m_Speed* fixedDeltaTime);
	if (rand() % 2 == 0) {
		m_EnemyTransform->MoveRight(-1, m_Speed / 3 * fixedDeltaTime);
	}
	else {
		m_EnemyTransform->MoveRight(1, m_Speed / 3 * fixedDeltaTime);
	}
}

void EnemyComp::PlayEnemySound(float fixedDeltaTime)
{
	m_TimeLastPlayedSound += fixedDeltaTime;

	if (m_TimeLastPlayedSound > m_TimePlayedSound)
	{
		m_TimeLastPlayedSound = 0;
		if (rand() % 2 == 0)
			Rev::Rev_CoreSystems::pRevSound->PlayRevSound("EnemyGrawl");
	}
}
