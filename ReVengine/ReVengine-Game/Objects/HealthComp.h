#pragma once

#include "GameObjects/BaseComponent.h"
#include <functional>

class HealthComp : public Rev::BaseComponent
{
public:
	HealthComp(Rev::GameObject* gameObj);
	~HealthComp();

	void SetHealth(float health);
	const float& GetHealth() { return m_CurrentHealth; }
	void SetMaxHealth(float maxHealth);
	const float& GetMaxHealth() { return m_MaxHealth; }

	void AddHealth(float addition);

	void SetDeathFunctionality(std::function<void()> deathFunc) { m_DeathFunc = deathFunc; };

private:
	float m_MaxHealth;
	float m_CurrentHealth;

	std::function<void()> m_DeathFunc;
};