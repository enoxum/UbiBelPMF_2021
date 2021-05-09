#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/atonement/atonement_controller.h"

#include "gameplay/atonement/systems/collision_handler_system.h"

using namespace dagger;
using namespace atonement;

void CollisionHandlerSystem::Run() {

        //dohvatamo igraca
        auto view = Engine::Registry().view<SimpleCollision, Transform, AtonementController::AtonementCharacter>();

        for (auto& entity : view) {
            //TODO: sta god treba
        }
}