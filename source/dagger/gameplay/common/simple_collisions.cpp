#include "simple_collisions.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animations.h"
#include "gameplay/PandemicShop/pandemic_character_controller.h"
#include "gameplay/PandemicShop/item.h"
#include "gameplay/PandemicShop/karen.h"

using namespace dagger;
using namespace pandemic;

void SimpleCollisionsSystem::Run()
{
    auto &reg = Engine::Registry();
    auto view = reg.view<SimpleCollision, Transform>();
    auto bot_view = reg.view<CollisionType::Char, PandemicKarenCharacter, AICommand>();
    auto hero_view = reg.view<CollisionType::Char, PandemicCharacter>();
    auto wall_view = reg.view<CollisionType::Wall>();
    auto item_view = reg.view<Item>();


    auto all_it = view.begin();
    while (all_it != view.end())
    {
        auto& col = view.get<SimpleCollision>(*all_it);
        col.colided = false;
        all_it++;
    }

    auto it = bot_view.begin();
    while(it != bot_view.end())
    {
        auto &collision = view.get<SimpleCollision>(*it);
        auto &transform = view.get<Transform>(*it);

        auto it2 = wall_view.begin();
        while(it2 != wall_view.end())
        {
            auto &col = view.get<SimpleCollision>(*it2);
            auto &tr = view.get<Transform>(*it2);
            
            // processing one collision per frame for each colider
            if (collision.IsCollided(transform.position, col, tr.position))
            {
                collision.colidedWith = *it2;   
                col.colidedWith = *it;

                collision.colided = true;
                col.colided = true;

                resolveDirection( collision, transform, col, tr);

            }
            it2++;
        }
        it2 = item_view.begin();
        while(it2 != item_view.end())
        {
            
            if(reg.has<SimpleCollision>(*it2)){
                auto &col = view.get<SimpleCollision>(*it2);
                auto &tr = view.get<Transform>(*it2);
                
                if (collision.IsCollided(transform.position, col, tr.position))
                {
                    collision.colidedWith = *it2;   
                    col.colidedWith = *it;

                    collision.colided = true;
                    col.colided = true;

                    auto &karen_command = reg.get<AICommand>(*it);
                    auto prob = rand() / (float) RAND_MAX;
                    Logger::info("\nPROB {}\n", prob);
                    if( prob > karen_command.pick_probability){
                        Logger::info("\nKaren pickup_prob {}\n", karen_command.pick_probability);
                        auto &item = reg.get<Item>(*it2);
                        auto &sprite = reg.get<Sprite>(*it2);
                        auto karen = KarenCharacter::Get(*it);
                        if(!item.hidden){
                            item.hidden = true;
                            item.pickable = true;
                            reg.remove<SimpleCollision>(*it2);
                            sprite.scale = {0, 0};
                            karen.inventory.emplace_back(*it2);
                            karen_command.picked = true;
                        }

                    }
                    resolveItem( collision, transform, col, tr, karen_command);
                    Logger::info("\nPICKED AFTER COLLISION: {}\n", karen_command.picked);

                }
            }
            it2++;
        }
        it++;
    }


    auto h_it = hero_view.begin();
    while(h_it != hero_view.end())
    {
        auto &collision = view.get<SimpleCollision>(*h_it);
        auto &transform = view.get<Transform>(*h_it);

        auto it2 = wall_view.begin();
        while(it2 != wall_view.end())
        {
            auto &col = view.get<SimpleCollision>(*it2);
            auto &tr = view.get<Transform>(*it2);
            
            // processing one collision per frame for each colider
            if (collision.IsCollided(transform.position, col, tr.position))
            {
                collision.colidedWith = *it2;   
                col.colidedWith = *h_it;

                collision.colided = true;
                col.colided = true;

                resolveDirection( collision, transform, col, tr);

            }
            it2++;
        }
        it2 = item_view.begin();
        while(it2 != item_view.end())
        {
            if(reg.has<SimpleCollision>(*it2)){
                auto &col = view.get<SimpleCollision>(*it2);
                auto &tr = view.get<Transform>(*it2);
                
                if (collision.IsCollided(transform.position, col, tr.position))
                {
                    collision.colidedWith = *it2;   
                    col.colidedWith = *h_it;

                    collision.colided = true;
                    col.colided = true;

                    resolveDirection( collision, transform, col, tr);

                }
            }
            it2++;
        }
        h_it++;
    }
}




void SimpleCollisionsSystem::resolveItem(SimpleCollision &collision, Transform &col_transform, 
                                            SimpleCollision &other, Transform& other_transform,
                                            AICommand& command ){
    
    
    if(command.picked){
        resolveDirection(collision, col_transform, other, other_transform);
        command.picked = false;
        other.colided = false;
    }
    else{
        resolveDirection(collision, col_transform, other, other_transform); 
        other.colided = false;
        collision.colided = false;
        command.next = {(rand() % AISystem::border_width) - AISystem::border_width, 
                        (rand() % AISystem::border_height) - AISystem::border_height};
        command.finishedX = false;
        command.finishedY = false;
        command.finished = false;
        
           
    }
}




void SimpleCollisionsSystem::resolveDirection(SimpleCollision &collision, Transform &col_transform, SimpleCollision &other, Transform& other_transform ){
    
    if (collision.GetCollisionSides(col_transform.position, collision, other_transform.position).x == 1){
        col_transform.position.x -= collision.size.x/10.f;
    }
    else if(collision.GetCollisionSides(col_transform.position, collision, other_transform.position).x == -1){
        col_transform.position.x += collision.size.x/10.f;
    }
    else if(collision.GetCollisionSides(col_transform.position, collision, other_transform.position).y == 1){
        col_transform.position.y -= collision.size.y/10.f;
    }
    else if(collision.GetCollisionSides(col_transform.position, collision, other_transform.position).y == -1){
        col_transform.position.y += collision.size.y/10.f;
    }
}
// SimpleCollision

bool SimpleCollision::IsCollided(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_)
{
    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    if (p.x < p2.x + other_.size.x &&
        p.x + size.x > p2.x &&
        p.y < p2.y + other_.size.y &&
        p.y + size.y > p2.y)
    {
        return true;
    }

    return false;
}

Vector2 SimpleCollision::GetCollisionSides(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_)
{
    Vector2 res(0, 0);

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    Float32 xDif = std::min(std::abs(p.x + size.x - p2.x), std::abs(p2.x + other_.size.x - p.x));
    Float32 yDif = std::min(std::abs(p.y + size.y - p2.y), std::abs(p2.y + other_.size.y - p.y));

    if (xDif < yDif)
    {
        res.x = std::abs(p.x + size.x - p2.x) < std::abs(p2.x + other_.size.x - p.x) ? 1 : -1;
    }
    else
    {
        res.y = std::abs(p.y + size.y - p2.y) < std::abs(p2.y + other_.size.y - p.y) ? 1 : -1;
    }

    return res;
}

Vector3 SimpleCollision::GetCollisionCenter(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_)
{
    Vector3 res;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    res.x = (p.x + size.x / 2.f + p2.x + other_.size.x) / 2.f;
    res.y = (p.y + size.y / 2.f + p2.y + other_.size.y) / 2.f;
    res.z = 1.f;

    return res;
}

