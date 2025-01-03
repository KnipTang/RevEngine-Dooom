#pragma once

#include <PxPhysicsAPI.h>
#include <iostream>
#include "GameObjects/Components/CompCollision.h"

namespace RevDev
{
    class CollisionCallback : public physx::PxSimulationEventCallback {
    public:

        void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {
            for (physx::PxU32 i = 0; i < nbPairs; i++) {
                if (pairs[i].events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                    void* userDataA = pairHeader.actors[0]->userData;
                    void* userDataB = pairHeader.actors[1]->userData;

                    auto* compA = static_cast<Rev::CompCollision*>(userDataA);
                    auto* compB = static_cast<Rev::CompCollision*>(userDataB);

                    if (compA) compA->OnContact(compB);
                    if (compB) compB->OnContact(compA);
                }
            }
        }
    
        void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override {
            for (physx::PxU32 i = 0; i < count; i++) {
                if (pairs[i].status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                    std::cout << "Trigger detected between objects! " 
                        << pairs[i].triggerActor << " by " << pairs[i].otherActor << std::endl;

                }
            }
        }
        void onConstraintBreak(physx::PxConstraintInfo* /*constraints*/, physx::PxU32 /*count*/) override {}
        void onWake(physx::PxActor** /*actors*/, physx::PxU32 /*count*/) override {}
        void onSleep(physx::PxActor** /*actors*/, physx::PxU32 /*count*/) override {}
        void onAdvance(const physx::PxRigidBody* const* /*bodyBuffer*/, const physx::PxTransform* /*poseBuffer*/, const physx::PxU32 /*count*/) override {};
    };
}