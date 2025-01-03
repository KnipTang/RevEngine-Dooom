#pragma once

#include "GameObjects/BaseComponent.h"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"

namespace Rev
{
	class CompTransform : public BaseComponent
	{
	public:
		CompTransform(GameObject* gameObj, glm::vec3 position = glm::vec3{0,0,0}, glm::vec3 rotation = glm::vec3{ 0, 0, 0 }, glm::vec3 scale = glm::vec3{ 1,1,1 });
		~CompTransform() {};

		void update([[maybe_unused]] float deltaTime) override;

		void SetPosition(float x, float y, float z);
		void SetPosition(glm::vec3 pos);
		glm::vec3& GetWorldPosition();
		glm::vec3& GetLocalPosition();

		void SetRotationRad(float x, float y, float z);
		void SetRotationRad(glm::vec3 dir);
		void SetRotationDegree(float x, float y, float z);
		void SetRotationDegree(glm::vec3 dir);
		glm::vec3& GetWorldRotation();
		glm::vec3& GetLocalRotation();

		glm::mat4& GetModelMatrix();

		const glm::vec3 GetForwardVector();
		const glm::vec3 GetRightVector();

		void Move(glm::vec3 dir, float speed = 1);
		void MoveForward(int input, float speed = 1);
		void MoveRight(int input, float speed = 1);

		void Turn(float x, float y);
		void AddYawInput(float input);
		void AddPitchInput(float input);

		bool IsPositionDirty() { return m_DirtyPosition; }
		bool IsRotationDirty() { return m_DirtyRotation; }
	private:
		void SetDirtyPosition();
		void SetDirtyRotation();

		void UpdatePosition();
		void UpdateRotation();
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale; //TODO

		glm::vec3 m_LocalPosition;
		glm::vec3 m_LocalRotation;

		glm::mat4 m_ModelMatrix;

		bool m_DirtyPosition;
		bool m_DirtyRotation;
	};
}