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
	class Physics_PhysX
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
		physx::PxScene* gScene = nullptr;

	private:
		physx::PxDefaultAllocator gAllocator;
		physx::PxDefaultErrorCallback gErrorCallback;
		physx::PxFoundation* gFoundation = nullptr;
		physx::PxPhysics* gPhysics = nullptr;
		physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
		physx::PxMaterial* gMaterial = nullptr;

		physx::PxPvd* gPvd = nullptr;

		RevDev::CollisionCallback* collisionCallback;

		std::unordered_map<int, physx::PxRigidActor*> m_Actors;
	};
}