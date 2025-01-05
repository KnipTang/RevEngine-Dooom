#pragma once

#include "GameObjects/BaseComponent.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <functional>

namespace Rev
{
	class Physics;
	class CompTransform;
}

namespace Rev
{
	class CompCollision final : public Rev::BaseComponent
	{
	public:
		CompCollision(Rev::GameObject* gameObj, Rev::Physics* physicsHandle, bool staticObject = false, bool gravity = false, glm::vec3 size = { 1,1,1 }, glm::vec3 pos = { 0,0,0 });
		~CompCollision();

		void LateUpdate([[maybe_unused]] float deltaTime) override;

		void SetOnContactFunction(std::function<void(Rev::CompCollision* other)> onContactFnc) { m_OnContactFunction = onContactFnc; }
		void OnContact(Rev::CompCollision* other);

		int GetID() { return m_ColliderID; }
	private:
		Rev::Physics* m_PhysicsHandle;
		std::function<void(Rev::CompCollision* other)> m_OnContactFunction;

		Rev::CompTransform* m_TransformHandle;

		glm::vec3 m_Position;
		glm::vec3 m_Size;

		int m_ColliderID;
		static int s_ColliderCounter;
	};
}