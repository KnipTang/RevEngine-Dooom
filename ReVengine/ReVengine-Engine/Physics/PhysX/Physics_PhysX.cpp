#include "Physics_PhysX.h"
#include "CollisionCallback.h"
#include <stdexcept>

using namespace RevDev;

Physics_PhysX::Physics_PhysX()
{

}

Physics_PhysX::~Physics_PhysX()
{
    for (int i = 0; i < m_Actors.size(); i++)
    {
        DestroyCollider(i);
    }

    m_Scene->release();
    m_Dispatcher->release();
    m_Physics->release();
    m_Foundation->release();
}

void Physics_PhysX::Init()
{
    m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
    if (!m_Foundation)
        throw std::runtime_error("gFoundation error!");

    m_Pvd = PxCreatePvd(*m_Foundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

    m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, m_Pvd);
    if (!m_Physics)
        throw std::runtime_error("gPhysics error!");

    m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

    m_CollisionCallback = new RevDev::CollisionCallback{};

    physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = m_Dispatcher;
    sceneDesc.filterShader = [](
        physx::PxFilterObjectAttributes /*attributes0*/,
        physx::PxFilterData /*filterData0*/,
        physx::PxFilterObjectAttributes /*attributes1*/,
        physx::PxFilterData /*filterData1*/,
        physx::PxPairFlags& pairFlags,
        const void* /*constantBlock*/,
        physx::PxU32 /*constantBlockSize*/) -> physx::PxFilterFlags 
        {
            pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

            pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
            pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;

            return physx::PxFilterFlag::eDEFAULT;
        };
    sceneDesc.simulationEventCallback = m_CollisionCallback;
    sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;

    m_Scene = m_Physics->createScene(sceneDesc);

    m_DefaultMaterial = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
    if (!m_DefaultMaterial)
        throw std::runtime_error("gMaterial error!");
}

void Physics_PhysX::Simulate(float fixedDeltaTime)
{
    m_Scene->simulate(fixedDeltaTime);
    m_Scene->fetchResults(true);
}

void Physics_PhysX::CreateStatic(int id, glm::vec3 pos, glm::vec3 size)
{
    physx::PxTransform PxPos(physx::PxVec3(pos.x, pos.y, pos.z));
    physx::PxRigidStatic* PxStatic = m_Physics->createRigidStatic(PxPos);

    physx::PxShape* PxShape = physx::PxRigidActorExt::createExclusiveShape(
        *PxStatic, 
        physx::PxBoxGeometry(size.x, size.y, size.z), 
        *m_DefaultMaterial);
    if (!PxShape)
        throw std::runtime_error("CreateExclusiveShape failed!");

    //PxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    PxShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
    PxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    PxShape->setFlag(physx::PxShapeFlag::eVISUALIZATION, true);


    m_Scene->addActor(*PxStatic);
    m_Actors.emplace(id, PxStatic);
}

void Physics_PhysX::CreateDynamic(int id, glm::vec3 pos, glm::vec3 size, bool gravity, void* userData)
{
    physx::PxTransform PxPos(physx::PxVec3(pos.x, pos.y, pos.z));
    physx::PxRigidDynamic* PxDynamic = m_Physics->createRigidDynamic(PxPos);

    physx::PxShape* PxShape = physx::PxRigidActorExt::createExclusiveShape(
        *PxDynamic, 
        physx::PxBoxGeometry(size.x, size.y, size.z), 
        *m_DefaultMaterial);
    if (!PxShape)
        throw std::runtime_error("Create shape error");

    if(!gravity)
        PxDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

   // PxDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
    //PxDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);

    physx::PxRigidBodyExt::updateMassAndInertia(*PxDynamic, 1.0f);

    PxDynamic->userData = userData;

    m_Scene->addActor(*PxDynamic);
    m_Actors.emplace(id, PxDynamic);
}

void Physics_PhysX::DestroyCollider(int id)
{
    auto it = m_Actors.find(id);
    if (it != m_Actors.end()) 
    {
        auto&& actor = it->second;
        m_Scene->removeActor(*actor);
        actor->release();
        m_Actors.erase(it);
    }
}

void Physics_PhysX::UpdateActorTransform(int id, glm::vec3 pos, glm::vec3 rot)
{
    physx::PxVec3 pxPosition(pos.x, pos.y, pos.z);

    physx::PxQuat pxRotation = physx::PxQuat(physx::PxIdentity);

    if (rot != glm::vec3(0.0f))
    {
        physx::PxQuat yaw(rot.y, physx::PxVec3(0, 1, 0));
        physx::PxQuat pitch(rot.x, physx::PxVec3(1, 0, 0));
        physx::PxQuat roll(rot.z, physx::PxVec3(0, 0, 1));

        pxRotation = yaw * pitch * roll;
    }

    physx::PxTransform transform(pxPosition, pxRotation);
    
    m_Actors.at(id)->setGlobalPose(transform, true);

    auto* rigidDynamic = m_Actors.at(id)->is<physx::PxRigidDynamic>();
    if (rigidDynamic)
    {
        rigidDynamic->clearForce(physx::PxForceMode::eFORCE);
        rigidDynamic->clearTorque();

        rigidDynamic->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
        rigidDynamic->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
    }
}