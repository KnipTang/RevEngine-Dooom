#pragma once

#include "GameObjects/BaseComponent.h"
#include <functional>

namespace Rev
{
	class GameObject;
}

class ObjectSpawnerComp : public Rev::BaseComponent
{
public:
	ObjectSpawnerComp(Rev::GameObject* gameObj);
	~ObjectSpawnerComp();

	void update([[maybe_unused]] float deltaTime) override;

	void SetSpawnCondition(std::function<bool()> spawnConditionFnc) { m_SpawnConditionFnc = spawnConditionFnc; }
	void SetObjectToSpawn(std::function<Rev::GameObject*()> obj) { m_Object = obj; }

private:
	std::function<bool()> m_SpawnConditionFnc;
	std::function<Rev::GameObject*()> m_Object;
};