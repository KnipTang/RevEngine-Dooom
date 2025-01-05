#include "CompCollision.h"
#include "Physics/Physics.h"
#include "Scenes/Scene.h"
#include <stdexcept>
#include "GameObjects/Components/CompTransform.h"

using namespace Rev;

int CompCollision::s_ColliderCounter = 0;

CompCollision::CompCollision(Rev::GameObject* gameObj, Rev::Physics* physicsHandle, bool staticObject, bool gravity, glm::vec3 size, glm::vec3 pos) :
    BaseComponent(gameObj),
    m_PhysicsHandle{ physicsHandle },
    m_TransformHandle{ m_GameObject->transform },
    m_Size{ size },
    m_ColliderID{ s_ColliderCounter++ }
{
    m_Position = m_TransformHandle->GetWorldPosition() + pos + (size * 0.5f);
    staticObject ? 
        m_PhysicsHandle->CreateStatic(m_ColliderID, m_Position, size) :
        m_PhysicsHandle->CreateDynamic(m_ColliderID, m_Position, size, gravity, this);
}

CompCollision::~CompCollision()
{
    
}

void CompCollision::LateUpdate([[maybe_unused]] float deltaTime)
{
    //if (m_TransformHandle->IsPositionDirty() || m_TransformHandle->IsRotationDirty())
        m_PhysicsHandle->UpdateActorTransform(m_ColliderID, m_TransformHandle->GetWorldPosition(), m_TransformHandle->GetWorldRotation());
}

void CompCollision::OnContact(Rev::CompCollision* other)
{
    if(m_OnContactFunction != nullptr) m_OnContactFunction(other);
}