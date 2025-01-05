#include "CompRender.h"
#include "Rev_CoreSystems.h"
#include "CompTransform.h"
#include "CompCamera.h"
#include "Rendering/Shaders/TextureShader.h"
#include "Rendering/Texture.h"
#include "glm/vec3.hpp"
#include "DirectXMath.h"
#include "GameObjects/GameObject.h"

using namespace Rev;

CompRender::CompRender(GameObject* gameObj, 
	CompCamera* camera, BaseShader* shader, Texture* texture, 
	float widthTexture, float heightTexture, 
	glm::vec3 position, bool transparent) :
	BaseComponent(gameObj),
	m_TransformComp{ gameObj->transform },
	m_CameraComp{ camera },
	m_Shader{ shader },
	m_Texture{ texture },
	m_MeshId{},
	m_Transparent{transparent}
{
	//const TextureData& textureData = *m_Texture->GetTextureDate();
	//float depthZ = m_TransformComp->GetWorldPosition().z;
	m_Vertices =
	{
		//Bottom Left
		{ { position.x, position.y, position.z }, { 0.f, 1.f } },
		//Bottom Right
		{ { position.x + widthTexture / 2,  position.y, position.z }, { 1.f, 1.f } },
		//Top Left
		{ { position.x, position.y + heightTexture / 2, position.z }, { 0.f, 0.f } },
		//Top Right
		{ { position.x + widthTexture / 2, position.y + heightTexture / 2, position.z }, { 1.f, 0.f } },
	};

	m_Indices =
	{
		0, 1, 2,
		2 ,1, 3,
	};

	m_MeshId = Rev_CoreSystems::pRevRender->AddMesh(m_Vertices, m_Indices);
}

const void CompRender::Render()
{
	DirectX::XMMATRIX modelMatrix = DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&m_TransformComp->GetModelMatrix()));
	m_Shader->SetShader(modelMatrix, m_CameraComp->GetCamera()->GetViewMatrix(), Rev::Rev_CoreSystems::pRevRender->GetProjectionMatrix(),
		m_Texture->GetShaderResourceView());
	m_Shader->SetShaderStages();
	Rev_CoreSystems::pRevRender->DrawMesh(m_MeshId);
}

float CompRender::GetDistanceToCamera()
{
	float distance = glm::distance(m_CameraComp->GetGameObject()->transform->GetWorldPosition(), m_TransformComp->GetWorldPosition());
	return distance;
}
