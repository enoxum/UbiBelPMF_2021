#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

#include "core/game/transforms.h"
#include "gameplay/atonement/systems/interaction_system.h"
#include "gameplay/atonement/components/marker_components.h"
#include "gameplay/atonement/systems/character_collisions.h"
#include <iostream>

using namespace dagger;
using namespace atonement;

void IntearactionSystem::SpinUp() 
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&IntearactionSystem::OnKeyboardEvent>(this);
}

void IntearactionSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&IntearactionSystem::OnKeyboardEvent>(this);
}

void IntearactionSystem::Run()
{   
    if (!loreStonePositionsLoaded) 
    {
        auto view = Engine::Registry().view<Transform, InteractableComponent>();
        for (auto& entity : view)
        {   
            auto& transf = view.get<Transform>(entity);
            auto& interact = view.get<InteractableComponent>(entity);
            loreStonePositions.push_back( std::make_pair(transf.position, interact.id) );
        }
        loreStonePositionsLoaded = true;
    }

    auto playerView = Engine::Registry().view<Transform, CharacterCollision>();
    for (auto& entity : playerView) 
    {   
        auto& transf = playerView.get<Transform>(entity);

        for (auto& stone : loreStonePositions) 
        {
            if (std::abs(transf.position.x - stone.first.x) < 250 &&
                std::abs(transf.position.y - stone.first.y) < 150)
            {   
                nearestLoreStone = stone;
                interactionInputEnabled = true;
                //iscrtavanje onog Press E sprajta
            }
            else 
            {
                interactionInputEnabled = false;
            }
        }
    }
}


void IntearactionSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    if (interactionInputEnabled && kEvent_.key == EDaggerKeyboard::KeyE)
    {
        //std::cout << "Pressed E\n";
        UnlockAbility(nearestLoreStone.second);
    }
}

void IntearactionSystem::UnlockAbility(int abilityId)
{
    // abilityId = 1  -> DASH
    // abilityId = 2  -> JUMP

    //std::cout << "ability unlocked: id = " << abilityId << std::endl;
}