#include "roboship_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

using namespace roboship;

void RoboshipControllerSystem::Run()
{
    Engine::Registry().view<RoboshipCharacterControllerFSM::StateComponent>()
        .each([&](RoboshipCharacterControllerFSM::StateComponent& state_)
            {
                characterFSM.Run(state_);
            });
}
