#include "ObjectSpawnerComp.h"
#include <Rev_CoreSystems.h>
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "GameObjects/GameObject.h"

ObjectSpawnerComp::ObjectSpawnerComp(Rev::GameObject* gameObj) :
	Rev::BaseComponent(gameObj)
{

}

ObjectSpawnerComp::~ObjectSpawnerComp()
{
}

void ObjectSpawnerComp::Update([[maybe_unused]] float deltaTime)
{
	if (m_SpawnConditionFnc())
		Rev::Rev_CoreSystems::pSceneManager->GetActiveScenes().at(0)->AddGameObject(m_Object());
}
