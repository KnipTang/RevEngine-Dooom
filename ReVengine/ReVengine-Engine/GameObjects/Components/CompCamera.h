#pragma once

#include "GameObjects/BaseComponent.h"
#include <memory>

namespace RevDev
{
	class Camera;
}

namespace Rev
{
	class CompTransform;
}

enum class RotationAxes
{
	x,
	y
};

namespace Rev
{
	class CompCamera final : public BaseComponent
	{
	public:
		CompCamera(GameObject* gameObj, Rev::CompTransform* transform, bool flipControlls = false);
		~CompCamera() = default;

		void LateUpdate([[maybe_unused]] float deltaTime) override;

		void Turn(float x, float y);

		void LockAxes(RotationAxes axe, bool lock = true);

		RevDev::Camera* GetCamera() const { return m_Camera.get(); }
	private:
		std::unique_ptr<RevDev::Camera> m_Camera;

		Rev::CompTransform* m_Transform;

		float m_LookSensitivity;

		bool m_ControllsFlipped;

		bool m_LockedX;
		bool m_LockedY;
	};
}