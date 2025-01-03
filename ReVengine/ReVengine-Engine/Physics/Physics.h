#pragma once

#include <memory>
#include <glm/vec3.hpp>

namespace RevDev 
{
	class PhysicsImpl;
}

namespace Rev
{
	class Physics
	{
	public:
		Physics();
		~Physics();

		void Init();
		void Simulate(float fixedDeltaTime);

		void CreateStatic(int id, glm::vec3 pos, glm::vec3 size);
		void CreateDynamic(int id, glm::vec3 pos, glm::vec3 size, bool gravity, void* userData);
		void DestroyCollider(int id);

		void UpdateActorTransform(int id, glm::vec3 pos, glm::vec3 rot);
	private:
		std::unique_ptr<RevDev::PhysicsImpl> m_PhysicsImpl;
	};
}