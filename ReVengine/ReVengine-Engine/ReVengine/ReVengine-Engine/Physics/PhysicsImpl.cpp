#include "PhysicsImpl.h"
#include "PhysX/Physics_PhysX.h"

using namespace RevDev;

PhysicsImpl::PhysicsImpl() :
    m_PhysicsPhysX{std::make_unique<RevDev::Physics_PhysX>()}
{
}

PhysicsImpl::~PhysicsImpl()
{

}

void PhysicsImpl::Init()
{
    m_PhysicsPhysX->Init();
}

void PhysicsImpl::Simulate(float fixedDeltaTime)
{
    m_PhysicsPhysX->Simulate(fixedDeltaTime);
}

void PhysicsImpl::CreateStatic(int id, glm::vec3 pos, glm::vec3 size)
{
    m_PhysicsPhysX->CreateStatic(id, pos, size);
}

void PhysicsImpl::CreateDynamic(int id, glm::vec3 pos, glm::vec3 size, bool gravity, void* userData)
{
    m_PhysicsPhysX->CreateDynamic(id, pos, size, gravity, userData);
}

void PhysicsImpl::DestroyCollider(int id)
{
    m_PhysicsPhysX->DestroyCollider(id);
}

void PhysicsImpl::UpdateActorTransform(int id, glm::vec3 pos, glm::vec3 rot)
{
    m_PhysicsPhysX->UpdateActorTransform(id, pos, rot);
}