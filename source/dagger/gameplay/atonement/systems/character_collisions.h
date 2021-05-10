#pragma once

#include "core/system.h"
#include "core/core.h"

#include "gameplay/common/simple_collisions.h"

using namespace dagger;

namespace atonement {

    enum class CollisionSide {
        Left,
        Right,
        Up,
        Down,
        None
    };

    struct CharacterCollision
    {
        Vector2 size;
        Vector2 pivot{ -0.5f, -0.5f };

        bool colidedLeft = false;
        entt::entity colidedWithLeft = entt::null;
        bool colidedRight = false;
        entt::entity colidedWithRight = entt::null;
        bool colidedUp = false;
        entt::entity colidedWithUp = entt::null;
        bool colidedDown = false;
        entt::entity colidedWithDown = entt::null;

        
        CollisionSide IsCollided(const Vector3& pos_, const Vector3& posOther_, const SimpleCollision& colOther_);

        //UNTESTED
        Vector3 GetCollisionCenter(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_);
    };

    class CharacterCollisionsSystem : public System
    {
    public:
        inline String SystemName() { return "Character Collisions System"; }

        void Run() override;
    };
}
