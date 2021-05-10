#include "groundedness_detection_system.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "gameplay/atonement/atonement_controller.h"
#include "gameplay/atonement/systems/character_collisions.h"

#include <iostream>

using namespace dagger;
using namespace atonement;

GroundednessDetectionSystem::GroundednessDetectionSystem()
{
    canBePaused = true;
}


void GroundednessDetectionSystem::Run()
{
    Engine::Registry().view<Transform, Sprite, CharacterCollision, AtonementController::AtonementCharacter>()
        .each([&](Transform& transform_, Sprite& sprite_, CharacterCollision& collision_, AtonementController::AtonementCharacter& char_)
        {
                bool tmp = char_.grounded;

                if(collision_.colidedDown)
                {
                    char_.grounded = true;
                }
                else {
                    char_.grounded = false;
                }

                
                if (char_.grounded != tmp) {
                    std::cout << "Char pos: " << transform_.position.x << " " << transform_.position.y << std::endl;
                    std::cout << "Grounded: " << (char_.grounded ? "true" : "false") << std::endl;
                }
        });


}