#pragma once
#include "GameObjects/BaseComponent.h"

namespace Rev
{
	class CompTransform;
}

class EnemyComp : public Rev::BaseComponent
{
public:
	EnemyComp(Rev::GameObject* gameObj);
	~EnemyComp();

	void fixedUpdate([[maybe_unused]] float fixedDeltaTime) override;

	void SetSpeed(float speed) { m_Speed = speed; }
private:
	void WalkToPlayer(float fixedDeltaTime);
	void PlayEnemySound(float fixedDeltaTime);
private:
	Rev::CompTransform* m_EnemyTransform;

	float m_TimeLastPlayedSound;
	float m_TimePlayedSound;

	float m_Speed;
};