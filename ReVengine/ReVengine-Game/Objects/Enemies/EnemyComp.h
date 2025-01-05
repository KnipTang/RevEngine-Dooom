#pragma once
#include "GameObjects/BaseComponent.h"

namespace Rev
{
	class CompTransform;
}

class EnemyComp final : public Rev::BaseComponent
{
public:
	EnemyComp(Rev::GameObject* gameObj);
	~EnemyComp();

	void FixedUpdate([[maybe_unused]] float fixedDeltaTime) override;

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