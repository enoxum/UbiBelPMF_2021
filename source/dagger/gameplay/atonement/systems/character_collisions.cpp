#include "gameplay/atonement/systems/character_collisions.h"
#include "gameplay/common/simple_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace atonement;

void CharacterCollisionsSystem::Run()
{
    auto view = Engine::Registry().view<CharacterCollision, Transform>();

    //samo nas lik ima CharacterCollision
    for (auto& character : view) {

        auto& charCollision = view.get<CharacterCollision>(character);
        auto& charTransform = view.get<Transform>(character);

        //resetujemo sve kolizije koje su postojale u proslom frejmu
        charCollision.colidedLeft = false;
        charCollision.colidedRight = false;
        charCollision.colidedUp = false;
        charCollision.colidedDown = false;

        charCollision.colidedWithLeft = entt::null;
        charCollision.colidedWithRight = entt::null;
        charCollision.colidedWithUp = entt::null;
        charCollision.colidedWithDown = entt::null;

        //svi ostali entiteti sa kolizijom
        auto simpleView = Engine::Registry().view<SimpleCollision, Transform>();
        for (auto& entity : simpleView) {

            auto& otherCollision = simpleView.get<SimpleCollision>(entity);
            auto& otherTransform = simpleView.get<Transform>(entity);
           
            CollisionSide collisionDetection = charCollision.IsCollided(charTransform.position, otherTransform.position, otherCollision);
            
            switch (collisionDetection) {
                case CollisionSide::Left: 
                    charCollision.colidedLeft = true;
                    charCollision.colidedWithLeft = entity;
                    break;
                case CollisionSide::Right:
                    charCollision.colidedRight = true;
                    charCollision.colidedWithRight = entity;
                    break;
                case CollisionSide::Up:
                    charCollision.colidedUp = true;
                    charCollision.colidedWithUp = entity;
                    break;
                case CollisionSide::Down:
                    charCollision.colidedDown = true;
                    charCollision.colidedWithDown = entity;
                    break;
                default: break;
            }
        }

    }
}

CollisionSide CharacterCollision::IsCollided(const Vector3& pos_, const Vector3& posOther_, const SimpleCollision& colOther_)
{
    Vector2 p(pos_.x, pos_.y);
    Vector2 p2(posOther_.x, posOther_.y);

    if (p.x < p2.x && ((p2.x - p.x) < (size.x + colOther_.size.x) / 2.f ) &&
        std::abs(p.y - p2.y) < (size.y + colOther_.size.y) / 2.f )
    {   
        
        if (p.y + size.y / 2.f < p2.y) { return CollisionSide::Up; }
        else if (p.y - size.y / 2.f > p2.y) { return CollisionSide::Down; }

        return CollisionSide::Right;
    }

    else if (p.x > p2.x && ((p.x - p2.x) < (size.x + colOther_.size.x) / 2.f) &&
        std::abs(p.y - p2.y) < (size.y + colOther_.size.y) / 2.f)
    {
        if (p.y + size.y / 2.f < p2.y) { return CollisionSide::Up; }
        else if (p.y - size.y / 2.f > p2.y) { return CollisionSide::Down; }

        return CollisionSide::Left;
    }

    /*else if (p.y < p2.y && ((p2.y - p.y) < (size.y + colOther_.size.y) / 2.) &&
        std::abs(p.x - p2.x) < (size.x + colOther_.size.x) / 2.)
    {
        return CollisionSide::Up;
    }

    else if (p.y > p2.y && ((p.y - p2.y) < (size.y + colOther_.size.y) / 2.) &&
        std::abs(p.x - p2.x) < (size.x + colOther_.size.x) / 2.)
    {
        return CollisionSide::Down;
    }*/

    return CollisionSide::None;
}

//UNTESTED
Vector3 CharacterCollision::GetCollisionCenter(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_)
{
    Vector3 res;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    res.x = (p.x + size.x / 2.f + p2.x + other_.size.x) / 2.f;
    res.y = (p.y + size.y / 2.f + p2.y + other_.size.y) / 2.f;
    res.z = 1.f;

    return res;
}
