#pragma once
#include "GameObjects/BaseComponent.h"
#include <functional>
#include <chrono>

namespace Rev
{
	class CompTransform;
}

class GunComp final : public Rev::BaseComponent
{
public:
	GunComp(Rev::GameObject* gameObj, Rev::CompTransform* playerTransform, float fireRate, std::function<Rev::GameObject*()> bulletFunc);
	~GunComp();

	void Update([[maybe_unused]] float deltaTime) override;

	void Fire();
	void SetAmmo(int ammo) { m_Ammo = float(ammo); }
	float& GetAmmo() { return m_Ammo; }
	
	void SetFireSoundEffect(const std::string name) { m_SoundEffectName = name; }

private:
	Rev::CompTransform* m_PlayerTransform;
	std::function<Rev::GameObject*()> m_BulletFunc;

	float m_FireRate;
	float m_TimeLastShot;
	bool m_ReadyToShoot;

	float m_Ammo;

	std::string m_SoundEffectName;
};