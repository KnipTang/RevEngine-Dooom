#include "GunComp.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "Rev_CoreSystems.h"
#include "GameObjects/Components/CompTransform.h"
#include "Rev_CoreSystems.h"

GunComp::GunComp(Rev::GameObject* gameObj, Rev::CompTransform* playerTransform, float fireRate, std::function<Rev::GameObject* ()> bulletFunc) :
	Rev::BaseComponent(gameObj),
	m_PlayerTransform{ playerTransform },
	m_BulletFunc{ bulletFunc },
	m_FireRate{ fireRate }
{

}

GunComp::~GunComp()
{

}

void GunComp::Update([[maybe_unused]] float deltaTime)
{
	if(!m_ReadyToShoot) 
		m_TimeLastShot += deltaTime;
	if (m_TimeLastShot >= m_FireRate)
		m_ReadyToShoot = true;
}

void GunComp::Fire()
{
	if (m_ReadyToShoot && m_Ammo != 0)
	{
		m_TimeLastShot = 0;
		m_ReadyToShoot = false;

		m_Ammo--;

		Rev::GameObject* bullet = m_BulletFunc();
		bullet->transform->SetPosition(m_PlayerTransform->GetWorldPosition());
		bullet->transform->SetRotationRad(m_PlayerTransform->GetWorldRotation());
		Rev::Rev_CoreSystems::pSceneManager->GetActiveScenes().at(0)->AddGameObject(bullet);

		if(!m_SoundEffectName.empty()) Rev::Rev_CoreSystems::pRevSound->PlayRevSound(m_SoundEffectName);
	}
}