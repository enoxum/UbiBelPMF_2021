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
#include "gameplay/brawler/entities/bullet.h"

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
            Vector2 position_ = { 0, 0 },
            Bool isLeft_ = false)
        {
            auto& reg = Engine::Registry();
            auto entity = reg.create();

            ATTACH_TO_FSM(BrawlerCharacterFSM, entity);

            auto chr = BrawlerCharacter::Get(entity);

            chr.sprite.scale = { isLeft_? -1 : 1, 1 };
            chr.sprite.position = { 0.0f, 0.0f, 0.0f };
            chr.sprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };

            chr.col.size = chr.sprite.size;

            AssignSprite(chr.sprite, "spritesheets:gunner:green:idle:1");
            AnimatorPlay(chr.animator, "gunner:green:IDLE");

            chr.transform.position = { position_.x, position_.y + chr.sprite.size.y/2, 1.0f };

            if (input_ != "")
                chr.input.contexts.push_back(input_);

            chr.col.size = { 24.0f, 30.0f };

            chr.character.startPosition = position_;
            chr.character.startDirection = isLeft_ ? -1 : 1;

            chr.character.currentWeapon = Engine::Registry().create();
            reg.get_or_emplace<Transform>(chr.character.currentWeapon);
            auto& x = reg.get_or_emplace<Sprite>(chr.character.currentWeapon);
            x.scale = { 1, 1 };
            x.position = { 0.0f, 0.0f, 0.0f };
            x.color = { 0.0f, 0.0f, 0.0f, 0.0f };
            AssignSprite(x, "EmptyWhitePixel");

            return chr;
        }
    };
}