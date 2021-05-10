#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/atonement/systems/character_collisions.h"
#include "gameplay/atonement/atonement_controller.h"
#include "gameplay/atonement/systems/collision_handler_system.h"
#include <iostream>

using namespace dagger;
using namespace atonement;

void CollisionHandlerSystem::Run() 
{
        auto view = Engine::Registry().view<CharacterCollision>();

        for (auto& entity : view) {

            auto& col = view.get<CharacterCollision>(entity);
            
            /*if (col.colidedLeft) {
                std::cout << "Collided LEFT " << int(entity) << " " << int(col.colidedWithLeft) << std::endl;
            }
            if (col.colidedRight) {
                std::cout << "Collided RIGHT " << int(entity) << " " << int(col.colidedWithRight) << std::endl;
            }
            if (col.colidedUp) {
                std::cout << "Collided UP " << int(entity) << " " << int(col.colidedWithUp) << std::endl;
            }
            if (col.colidedDown) {
                std::cout << "Collided DOWN " << int(entity) << " " << int(col.colidedWithDown) << std::endl;
            }*/

        }
}