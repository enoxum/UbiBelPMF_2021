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
    auto bot_view = reg.view<CollisionType::Char, PandemicKarenCharacter>();
    auto hero_view = reg.view<CollisionType::Char, PandemicCharacter>();
    auto wall_view = reg.view<CollisionType::Wall>();
    auto item_view = reg.view<CollisionType::Item>();


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

                    resolveDirection( collision, transform, col, tr);

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

   

//     // Find collisions Slimes
//     auto it1 = viewSlimes.begin();
//     while (it1 != viewSlimes.end())
//     {
//         auto& col1 = viewAll.get<Collision>(*it1);
//         auto& tr1  = viewAll.get<Transform>(*it1);

//         // Find collision with walls
//         auto it2 = viewWalls.begin();
//         while (it2 != viewWalls.end())
//         {            
//             auto& col2 = viewAll.get<Collision>(*it2);
//             auto& tr2  = viewAll.get<Transform>(*it2);
//             if (col2.size.x == 0 || col1.size.x == 0) {
//                 it2++;
//                 continue;
//             }
//             // processing one collision per frame for each colider
//             if (col1.IsCollided(tr1.position, col2, tr2.position))
//             {
//                 ResolveCharWall(col1, col2, tr1, tr2);
//             }
//             it2++;
//         }

//         // Find colliion with chars
//         it2 = viewChars.begin();
//         while (it2 != viewWalls.end())
//         {
//             auto& col2 = viewAll.get<Collision>(*it2);
//             auto& tr2 = viewAll.get<Transform>(*it2);
//             if (col2.size.x == 0 || col1.size.x == 0) {
//                 it2++;
//                 continue;
//             }
//             // processing one collision per frame for each colider
//             if (col1.IsCollided(tr1.position, col2, tr2.position))
//             {
//                 ResolveCharChar(col2, col1, tr2, tr1);
//             }
//             it2++;
//         }

//         //if (col1.colided) Logger::info(*it1);
//         it1++;
//     }

//     // Find collisions Attack
//     auto itA = viewAttack.begin();
//     while (itA != viewAttack.end())
//     {
//         auto& colA = viewAll.get<Collision>(*itA);
//         auto& trA = viewAll.get<Transform>(*itA);

//         // Find collision with Health
//         auto it2 = viewHealth.begin();
//         while (it2 != viewHealth.end())
//         {
//             auto& col2 = viewAll.get<Collision>(*it2);
//             auto& tr2 = viewAll.get<Transform>(*it2);
//             if (col2.size.x == 0 || colA.size.x == 0) {
//                 it2++;
//                 continue;
//             }
//             // processing one collision per frame for each colider
//             if (colA.IsCollided(trA.position, col2, tr2.position))
//             {
//                 auto& attackEnt = viewAttack.get<CollisionType::Attack>(*itA);
//                 Entity p;

//                 if ((int)(*it2) != (int)attackEnt.orig) {
//                     auto& attack = Engine::Registry().get<Attack>(attackEnt.orig);

//                     attack.damaged.push_back(*it2);
//                 }
//             }
//             it2++;
//         }

//         //if (col1.colided) Logger::info(*it1);
//         itA++;
//     }

//     // Find collisions player
//     auto itC = viewChars.begin();
    
//         auto& colC = viewAll.get<Collision>(*itC);
//         auto& trC = viewAll.get<Transform>(*itC);

//         // Find collision with walls
//         auto it2 = viewWalls.begin();
//         while (it2 != viewWalls.end())
//         {
//             auto& col2 = viewAll.get<Collision>(*it2);
//             auto& tr2 = viewAll.get<Transform>(*it2);
//             if (col2.size.x == 0 || colC.size.x == 0) {
//                 it2++;
//                 continue;
//             }
//             // processing one collision per frame for each colider
//             if (colC.IsCollided(trC.position, col2, tr2.position))
//             {
//                 ResolveCharWall(colC, col2, trC, tr2);
//             }
//             it2++;
//         }
   
// }



























void SimpleCollisionsSystem::resolveDirection(SimpleCollision &collision, Transform &col_transform, SimpleCollision &other, Transform& other_transform ){
    // if (collision.IsCollided(col_transform.position,other, other_transform.position))
    // {
    //     Vector2 colSides = collision.GetCollisionSides(col_transform.position, other, other_transform.position);
    //     col_transform.position.x -= colSides.x / 10;
    //     col_transform.position.y -= colSides.y / 10;
    // } 
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

