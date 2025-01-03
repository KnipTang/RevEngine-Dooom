#pragma once

#include <directxmath.h>
#include "glm/vec3.hpp"
#include <glm/gtc/quaternion.hpp>

namespace RevDev
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void Update(glm::vec3 position, glm::vec3 rotation);

		DirectX::XMMATRIX GetViewMatrix() const { return m_ViewMatrix; };
	private:

		DirectX::XMMATRIX m_ViewMatrix;
	};
}