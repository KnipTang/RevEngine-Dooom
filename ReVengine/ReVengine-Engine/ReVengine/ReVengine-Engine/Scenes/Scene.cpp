#include "Scene.h"
#include "GameObjects/GameObject.h"
#include "Rev_CoreSystems.h"
#include "Physics/Physics.h"
#include "GameObjects/Components/CompCollision.h"
#include "GameObjects/Components/CompRender.h"

using namespace Rev;

int Scene::sceneIDCounter = 0;

Scene::Scene() :
	sceneID{ sceneIDCounter++ },
	m_Physics{ std::make_unique<Rev::Physics>() }
{
	m_Physics->Init();
}

Scene::~Scene()
{
}

void Scene::update(float deltaTime)
{
	for (auto&& obj : m_AllGameObjects)
	{
		if(obj->IsActive()) obj->update(deltaTime);
	}
}

void Scene::lateUpdate(float deltaTime)
{
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj->IsActive()) obj->lateUpdate(deltaTime);
	}

	RemoveObjects();
}

void Scene::fixedUpdate(float fixedDeltaTime)
{
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj->IsActive()) obj->fixedUpdate(fixedDeltaTime);
	}
}

void Scene::Physics(float fixedDeltaTime)
{
	m_Physics->Simulate(fixedDeltaTime);
}

const void Scene::render()
{
	SortRenderObjects();
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj->IsActive()) obj->render();
	}
}

const GameObject* Scene::addGameObject(std::unique_ptr<GameObject> gameObj)
{
	m_AllGameObjects.emplace_back(std::move(gameObj));

	return m_AllGameObjects.back().get();
}

const GameObject* Scene::addGameObject(GameObject* gameObj)
{
	m_AllGameObjects.emplace_back(std::unique_ptr<GameObject>(gameObj));

	return m_AllGameObjects.back().get();
}

void Scene::SetActive(bool active)
{
	m_Active = active;
	if (active)
		Rev::Rev_CoreSystems::pSceneManager->AddActiveScene(this);
	else
		Rev::Rev_CoreSystems::pSceneManager->RemoveActiveScene(this);
}

void Scene::RemoveObjects()
{
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj == nullptr) continue;
		if(obj->IsActive() && obj->ToBeDestroyed()) 
			removeGameObject(obj.get());
	}
}

void Scene::removeGameObject(GameObject* obj)
{
	for (auto&& child : obj->GetChildren())
	{
		obj->RemoveChild(child.get());
	}

	auto&& collider = obj->getComponent<Rev::CompCollision>();
	if (collider != nullptr)
		m_Physics->DestroyCollider(collider->GetID());

	m_AllGameObjects.erase(
		std::remove_if(
			m_AllGameObjects.begin(),
			m_AllGameObjects.end(),
			[obj](const std::unique_ptr<GameObject>& gameObject) {
				return gameObject.get() == obj;
			}),
		m_AllGameObjects.end());
}

void Scene::SortRenderObjects()
{
	std::sort(m_AllGameObjects.begin(), m_AllGameObjects.end(), [](const std::unique_ptr<Rev::GameObject>& a, const std::unique_ptr<Rev::GameObject>& b) {
		auto&& renderCompA = a->getComponent<Rev::CompRender>();
		auto&& renderCompB = b->getComponent<Rev::CompRender>();

		if (!renderCompA || !renderCompB) {
			return renderCompA != nullptr;
		}

		if (renderCompA->m_Transparent != renderCompB->m_Transparent)
		{
			return renderCompA->m_Transparent < renderCompB->m_Transparent;
		}
		if (renderCompA->m_Is2D != renderCompB->m_Is2D) {
			return !renderCompA->m_Is2D;
		}
		return renderCompA->GetDistanceToCamera() > renderCompB->GetDistanceToCamera();
		});
}