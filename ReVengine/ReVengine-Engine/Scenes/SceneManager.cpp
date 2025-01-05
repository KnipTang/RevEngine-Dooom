#include "SceneManager.h"
#include "Scenes/Scene.h"
#include "Physics/Physics.h"

using namespace Rev;

int SceneManager::s_SceneManagerIDCounter = 0;

SceneManager::SceneManager() :
	m_SceneManagerID{ s_SceneManagerIDCounter++ },
	m_Physics{ std::make_unique<Rev::Physics>() }
{
	m_Physics->Init();
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(float deltaTime)
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->Update(deltaTime);
	}
}

void SceneManager::LateUpdate(float deltaTime)
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->LateUpdate(deltaTime);
	}
}

void SceneManager::FixedUpdate(float fixedDeltaTime)
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->FixedUpdate(fixedDeltaTime);
	}
}

void SceneManager::Physics(float fixedDeltaTime)
{
	m_Physics->Simulate(fixedDeltaTime);
}

const void SceneManager::Render()
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->Render();
	}
}

const Scene* SceneManager::AddScene(std::unique_ptr<Scene> scene)
{
	m_AllScenes.emplace_back(std::move(scene));

	return m_AllScenes.back().get();
}

std::vector<Scene*> SceneManager::GetActiveScenes()
{
	return m_ActiveScenes;
}

Scene* SceneManager::GetSceneByID(int ID)
{
	for (auto&& scene : m_AllScenes)
	{
		if (scene->GetID() == ID)
			return scene.get();
	}
	return nullptr;
}

Scene* SceneManager::GetSceneByTag(const std::string& tag)
{
	for (auto&& scene : m_AllScenes)
	{
		if (scene->m_Tag == tag)
			return scene.get();
	}
	return nullptr;
}