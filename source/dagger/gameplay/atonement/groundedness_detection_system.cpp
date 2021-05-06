#include "groundedness_detection_system.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/atonement/atonement_controller.h"

#include <iostream>

using namespace dagger;
using namespace atonement;

GroundednessDetectionSystem::GroundednessDetectionSystem()
{
    canBePaused = true;
}


void GroundednessDetectionSystem::Run()
{
    Engine::Registry().view<Sprite, AtonementController::AtonementCharacter>()
        .each([&](Sprite& sprite_, AtonementController::AtonementCharacter& char_)
        {
                bool tmp = char_.grounded;

                // TODO: use actual collisions
                if (sprite_.position.y <= -200) {
                    char_.grounded = true;
                }
                else {
                    char_.grounded = false;
                }

                
                if (char_.grounded != tmp) {
                    std::cout << "Char pos: " << sprite_.position.x << " " << sprite_.position.y << std::endl;
                    std::cout << "Grounded: " << (char_.grounded ? "true" : "false") << std::endl;
                }
        });


}