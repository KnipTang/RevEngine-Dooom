#pragma once

#include <vector>
#include <memory>

namespace Rev
{
	class Scene;
}

namespace Rev
{
	template <class T>
	concept sceneConcept = std::derived_from<T, Scene>;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void update(float deltaTime);
		void lateUpdate(float deltaTime);
		void fixedUpdate(float fixedDeltaTime);
		void Physics(float fixedDeltaTime);
		const void render();

		const Scene* addScene(std::unique_ptr<Scene> scene);

		template <sceneConcept T>
		T* getScene()
		{
			for (auto& scene : m_AllScenes)
			{
				if (auto derivedComp = dynamic_cast<T*>(scene.get()))
					return derivedComp;
			}

			return nullptr;
		}

		template <sceneConcept T>
		void removeScene()
		{
			m_AllScenes.erase(
				std::remove_if(m_AllScenes.begin(), m_AllScenes.end(),
					[](const std::unique_ptr<Scene>& scene) {
						return dynamic_cast<T*>(scene.get()) != nullptr;
					}),
				m_AllScenes.end());
		}

		void AddActiveScene(Scene* scene)
		{
			if (!IsSceneActive(scene))
				m_ActiveScenes.emplace_back(scene);
		}
		void RemoveActiveScene(Scene* scene)
		{
			if (IsSceneActive(scene))
				m_ActiveScenes.erase(std::find(m_ActiveScenes.begin(), m_ActiveScenes.end(), scene));
		}
		std::vector<Scene*> GetActiveScenes();

		const int GetID() { return sceneManagerID; }

	private:
		bool IsSceneActive(Scene* scene)
		{
			for (const auto& actScene : m_ActiveScenes)
			{
				if (actScene == scene)
					return true;
			}
			return false;
		}
	private:
		std::vector<std::unique_ptr<Scene>> m_AllScenes;
		std::vector<Scene*> m_ActiveScenes;

		static int sceneManagerIDCounter;
		int sceneManagerID;
	};
}