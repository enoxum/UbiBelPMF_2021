#include "gameplay/brawler/components/weapon_collision.h"

using namespace brawler;
using namespace dagger;

bool WeaponCollision::IsCollided(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_)
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

// Vector2 WeaponCollision::GetCollisionSides(const Vector3& pos_, const WeaponCollision& other_, const Vector3& posOther_)
// {
//     Vector2 res(0, 0);

//     Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
//     Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

//     Float32 xDif = std::min(std::abs(p.x + size.x - p2.x), std::abs(p2.x + other_.size.x - p.x));
//     Float32 yDif = std::min(std::abs(p.y + size.y - p2.y), std::abs(p2.y + other_.size.y - p.y));

//     if (xDif < yDif)
//     {
//         res.x = std::abs(p.x + size.x - p2.x) < std::abs(p2.x + other_.size.x - p.x) ? 1 : -1;
//     }
//     else
//     {
//         res.y = std::abs(p.y + size.y - p2.y) < std::abs(p2.y + other_.size.y - p.y) ? 1 : -1;
//     }

//     return res;
// }

// Vector3 WeaponCollision::GetCollisionCenter(const Vector3& pos_, const WeaponCollision& other_, const Vector3& posOther_)
// {
//     Vector3 res;

//     Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
//     Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

//     res.x = (p.x + size.x / 2.f + p2.x + other_.size.x) / 2.f;
//     res.y = (p.y + size.y / 2.f + p2.y + other_.size.y) / 2.f;
//     res.z = 1.f;

//     return res;
// }
