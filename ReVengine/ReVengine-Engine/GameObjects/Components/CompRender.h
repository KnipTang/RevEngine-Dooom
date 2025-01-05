#pragma once

#include "GameObjects/BaseComponent.h"
#include <vector>
#include <string>
#include "Utils/Vertex.h"
#include <glm/vec3.hpp>

//struct Vertex;
namespace Rev
{
	class CompTransform;
	class CompCamera;
	class BaseShader;
	class Texture;
}

namespace Rev
{
	class CompRender final : public BaseComponent
	{
	public:
		CompRender(GameObject* gameObj, CompCamera* camera, 
			BaseShader* shader, Texture* texture, 
			float widthTexture = 1, float heightTexture = 1,
			glm::vec3 position = {0,0,0}, bool transparent = false);
		~CompRender() {};

		const void Render() override;

		float GetDistanceToCamera();
	public:
		bool m_Transparent;

		bool m_Is2D;
	private:
		CompTransform* m_TransformComp;
		CompCamera* m_CameraComp;

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned short> m_Indices;

		BaseShader* m_Shader;
		Texture* m_Texture;

		uint32_t m_MeshId;
	};
}