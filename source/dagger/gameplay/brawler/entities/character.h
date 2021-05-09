#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/game/finite_state_machine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/entities/character_fsm.h"

using namespace dagger;

namespace brawler
{

    struct BrawlerCharacter
    {
        Entity entity;
        Sprite& sprite;
        Animator& animator;
        InputReceiver& input;
        Player& character;
        Transform& transform;
        Movable& movable;
        SimpleCollision& col;

        static BrawlerCharacter Get(Entity entity)
        {
            auto& reg = Engine::Registry();
            auto& sprite = reg.get_or_emplace<Sprite>(entity);
            auto& anim = reg.get_or_emplace<Animator>(entity);
            auto& input = reg.get_or_emplace<InputReceiver>(entity);
            auto& character = reg.get_or_emplace<Player>(entity);
            auto& transform = reg.get_or_emplace<Transform>(entity);
            auto& movable = reg.get_or_emplace<Movable>(entity);
            auto& col = reg.get_or_emplace<SimpleCollision>(entity);

            return BrawlerCharacter{ entity, sprite, anim, input, character, transform, movable, col };
        }

        static BrawlerCharacter Create(
            String input_ = "",
            ColorRGB color_ = { 1, 1, 1 },
            Vector2 position_ = { 0, 0 },
            Bool isLeft = false)
        {
            auto& reg = Engine::Registry();
            auto entity = reg.create();

            ATTACH_TO_FSM(BrawlerCharacterFSM, entity);

            auto chr = BrawlerCharacter::Get(entity);

            chr.sprite.scale = { isLeft? -1 : 1, 1 };
            chr.sprite.position = { 0.0f, 0.0f, 0.0f };
            chr.sprite.color = { color_, 1.0f };

            AssignSprite(chr.sprite, "spritesheets:2lisp:Gunner_Green_Idle:idle:1");
            AnimatorPlay(chr.animator, "Gunner_Green:IDLE");

            chr.transform.position = { position_.x, position_.y + chr.sprite.size.y/2, 0.0f };

            if (input_ != "")
                chr.input.contexts.push_back(input_);

            chr.col.size = { 24.0f, 30.0f };

            return chr;
        }
    };

}