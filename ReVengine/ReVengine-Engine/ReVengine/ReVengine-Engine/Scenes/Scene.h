#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "GameObjects/GameObject.h"

namespace Rev 
{
	class GameObject;
}

namespace Rev
{
	class Physics;
}

namespace Rev
{
	template <class T>
	concept gameObjectConcept = std::derived_from<T, GameObject>;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void update(float deltaTime);
		void lateUpdate(float deltaTime);
		void fixedUpdate(float fixedDeltaTime);
		void Physics(float fixedDeltaTime);
		const void render();

		const GameObject* addGameObject(std::unique_ptr<GameObject> gameObj);
		const GameObject* addGameObject(GameObject* gameObj);

		template <gameObjectConcept T>
		const bool hasGameObject()
		{
			for (const auto& obj : m_AllGameObjects)
			{
				if (dynamic_cast<T*>(obj.get()))
					return true;
			}

			return false;
		}

		template <gameObjectConcept T>
		T* getGameObject()
		{
			for (auto& obj : m_AllGameObjects)
			{
				if (auto derivedComp = dynamic_cast<T*>(obj.get()))
					return derivedComp;
			}

			return nullptr;
		}

		void removeGameObject(GameObject* obj);

		void DisplaySceneHierarchy()
		{
			std::printf("Scene Hierachy: %s\tSceneID: %i\n", typeid(*this).name(), sceneID);
			std::ranges::for_each(m_AllGameObjects,
				[](std::unique_ptr<GameObject>& obj) -> void
				{
					obj->DisplayHierarchy();
				}
			);
		}

		Rev::Physics* getPhysicsHandle() { return m_Physics.get(); }

		const int GetID() { return sceneID; }
		
		void SetActive(bool active);;
		bool IsActive() { return m_Active; }

		void AddActiveGameObject(GameObject* object)
		{
			if (!IsGameObjectActive(object))
				m_ActiveGameObjects.emplace_back(object);
		}
		void RemoveActiveGameObject(GameObject* object)
		{
			if (IsGameObjectActive(object))
				m_ActiveGameObjects.erase(std::find(m_ActiveGameObjects.begin(), m_ActiveGameObjects.end(), object));
		}
		std::vector<Scene*> GetActiveScenes();
	private:
		bool IsGameObjectActive(GameObject* object)
		{
			for (const auto& actObject : m_ActiveGameObjects)
			{
				if (actObject == object)
					return true;
			}
			return false;
		}

		void RemoveObjects();

		void SortRenderObjects();
	private:
		std::vector<std::unique_ptr<GameObject>> m_AllGameObjects;
		std::vector<GameObject*> m_ActiveGameObjects;

		bool m_Active;

		static int sceneIDCounter;
		int sceneID;

		std::unique_ptr<Rev::Physics> m_Physics;
	};
}