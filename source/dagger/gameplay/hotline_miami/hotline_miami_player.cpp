#include "hotline_miami_player.h"
#include "gameplay/hotline_miami/hotline_miami_obstacle.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include <execution>

using namespace dagger;
using namespace hotline_miami;

void HotlineMiamiPlayerObstacleCollisionSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<HotlineMiamiPlayer, Transform, SimpleCollision>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& player = view.get<HotlineMiamiPlayer>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith))
            {
                SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                Transform& transform = viewCollisions.get<Transform>(col.colidedWith);

                Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

                do
                {
                    // get back for 1 frame 
                    Float32 dt = Engine::DeltaTime();
                   
                    if (collisionSides.x > 0)
                    {
                        t.position.x -= player.player_speed * dt;
                    }
                    else if (collisionSides.x < 0)
                    {
                        t.position.x += player.player_speed * dt;
                    }

                    if (collisionSides.y > 0)
                    {
                        t.position.y -= player.player_speed * dt;
                    }
                    else if (collisionSides.y < 0)
                    {
                        t.position.y += player.player_speed * dt;
                    }

                } while (col.IsCollided(t.position, collision, transform.position));
            }

            col.colided = false;
        }
    }
}
