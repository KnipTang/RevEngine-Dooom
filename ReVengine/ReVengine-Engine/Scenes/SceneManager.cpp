#include "SceneManager.h"
#include "Scenes/Scene.h"

using namespace Rev;

int SceneManager::sceneManagerIDCounter = 0;

SceneManager::SceneManager() :
	sceneManagerID{ sceneManagerIDCounter++ }
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::update(float deltaTime)
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->update(deltaTime);
	}
}

void SceneManager::lateUpdate(float deltaTime)
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->lateUpdate(deltaTime);
	}
}

void SceneManager::fixedUpdate(float fixedDeltaTime)
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->fixedUpdate(fixedDeltaTime);
	}
}

void SceneManager::Physics(float fixedDeltaTime)
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->Physics(fixedDeltaTime);
	}
}

const void SceneManager::render()
{
	for (auto&& scene : m_ActiveScenes)
	{
		scene->render();
	}
}

const Scene* SceneManager::addScene(std::unique_ptr<Scene> scene)
{
	scene->SetActive(true);

	m_AllScenes.emplace_back(std::move(scene));

	return m_AllScenes.back().get();
}

std::vector<Scene*> SceneManager::GetActiveScenes()
{
	return m_ActiveScenes;
}