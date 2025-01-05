#include "Scene.h"
#include "GameObjects/GameObject.h"
#include "Physics/Physics.h"
#include "Rev_CoreSystems.h"
#include "GameObjects/Components/CompCollision.h"
#include "GameObjects/Components/CompRender.h"

using namespace Rev;

int Scene::s_SceneIDCounter = 0;

Scene::Scene() :
	m_SceneID{ s_SceneIDCounter++ },
	m_Physics{ Rev::Rev_CoreSystems::pSceneManager->GetPhysicsHandle() },
	m_Tag{"NONE"}
{
}

Scene::Scene(std::string tag) :
	m_SceneID{ s_SceneIDCounter++ },
	m_Physics{ Rev::Rev_CoreSystems::pSceneManager->GetPhysicsHandle() },
	m_Tag{ tag }
{
}

Scene::~Scene()
{
}

void Scene::Update(float deltaTime)
{
	for (auto&& obj : m_AllGameObjects)
	{
		if(obj->IsActive()) obj->Update(deltaTime);
	}
}

void Scene::LateUpdate(float deltaTime)
{
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj->IsActive()) obj->LateUpdate(deltaTime);
	}

	RemoveAllObjects();
}

void Scene::FixedUpdate(float fixedDeltaTime)
{
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj->IsActive()) obj->FixedUpdate(fixedDeltaTime);
	}
}

const void Scene::Render()
{
	SortRenderObjects();
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj->IsActive()) obj->Render();
	}
}

const GameObject* Scene::AddGameObject(std::unique_ptr<GameObject> gameObj)
{
	m_AllGameObjects.emplace_back(std::move(gameObj));

	return m_AllGameObjects.back().get();
}

const GameObject* Scene::AddGameObject(GameObject* gameObj)
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

void Scene::RemoveAllObjects()
{
	for (auto&& obj : m_AllGameObjects)
	{
		if (obj == nullptr) continue;
		if(obj->IsActive() && obj->ToBeDestroyed()) 
			RemoveGameObject(obj.get());
	}
}

void Scene::RemoveGameObject(GameObject* obj)
{
	for (auto&& child : obj->GetChildren())
	{
		obj->RemoveChild(child.get());
	}

	auto&& collider = obj->GetComponent<Rev::CompCollision>();
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
		auto&& renderCompA = a->GetComponent<Rev::CompRender>();
		auto&& renderCompB = b->GetComponent<Rev::CompRender>();

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