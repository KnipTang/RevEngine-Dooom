#pragma once

#include <PxPhysicsAPI.h>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

namespace RevDev
{
	class CollisionCallback;
}

namespace RevDev
{
	class Physics_PhysX final
	{
	public:
		Physics_PhysX();
		~Physics_PhysX();

		void Init();
		void Simulate(float fixedDeltaTime);

		void CreateStatic(int id, glm::vec3 pos, glm::vec3 size);
		void CreateDynamic(int id, glm::vec3 pos, glm::vec3 size, bool gravity, void* userData);

		void DestroyCollider(int id);

		void UpdateActorTransform(int id, glm::vec3 pos, glm::vec3 rot);
	public:
		physx::PxScene* m_Scene = nullptr;

	private:
		physx::PxDefaultAllocator m_Allocator;
		physx::PxDefaultErrorCallback m_ErrorCallback;
		physx::PxFoundation* m_Foundation = nullptr;
		physx::PxPhysics* m_Physics = nullptr;
		physx::PxDefaultCpuDispatcher* m_Dispatcher = nullptr;
		physx::PxMaterial* m_DefaultMaterial = nullptr;

		physx::PxPvd* m_Pvd = nullptr;

		RevDev::CollisionCallback* m_CollisionCallback;

		std::unordered_map<int, physx::PxRigidActor*> m_Actors;
	};
}