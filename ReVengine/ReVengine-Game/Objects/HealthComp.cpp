#include "HealthComp.h"
#include <algorithm>

HealthComp::HealthComp(Rev::GameObject* gameObj) :
	Rev::BaseComponent(gameObj),
	m_CurrentHealth{100},
	m_MaxHealth{100}
{

}

HealthComp::~HealthComp()
{
}

void HealthComp::SetHealth(float health)
{
	m_CurrentHealth = std::clamp(health, 0.0f, m_MaxHealth);

	if (m_CurrentHealth == 0)
		m_DeathFunc();
}

void HealthComp::SetMaxHealth(float maxHealth)
{
	m_MaxHealth = maxHealth;
}

void HealthComp::AddHealth(float addition)
{
	SetHealth(m_CurrentHealth + addition);
}