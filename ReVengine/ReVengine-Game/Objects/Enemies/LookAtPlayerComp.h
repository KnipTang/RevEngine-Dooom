#pragma once
#include "GameObjects/BaseComponent.h"

namespace Rev
{
	class CompTransform;
}

class LookAtPlayerComp : public Rev::BaseComponent
{
public:
	LookAtPlayerComp(Rev::GameObject* gameObj, Rev::CompTransform* playerTransform);
	~LookAtPlayerComp();

	void update([[maybe_unused]] float deltaTime) override;

private:
	void TurnToPlayer();

private:
	Rev::CompTransform* m_PlayerTransform;
	Rev::CompTransform* m_ObjectTransform;
};