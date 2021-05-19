#include "hotline_miami_health.h"
#include "gameplay/hotline_miami/hotline_miami_player.h"
#include "gameplay/hotline_miami/hotline_miami_tools.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include <algorithm>
#include <execution>


using namespace dagger;
using namespace hotline_miami;

void HotlineMiamiHealthSystem::Run()
{
    auto players = Engine::Registry().view<Transform, Sprite, HotlineMiamiPlayer>();
    auto view = Engine::Registry().view<Transform, Sprite, HotlineMiamiHealth>();
    for (auto entity : view)
    {
        auto& healt = view.get<HotlineMiamiHealth>(entity);

        for (auto p_entity : players)
        {
        
            auto& sprite = view.get<Sprite>(entity);
            auto& t = view.get<Transform>(entity);

            auto& player = players.get<HotlineMiamiPlayer>(p_entity);
            auto& p_transform = players.get<Transform>(p_entity);
            auto& p_sprite = players.get<Sprite>(p_entity);

            if (player.health == 3)
            {
                AssignSprite(sprite, "hotline_miami:Health:health_3");
            }
            else if (player.health == 2)
            {
                AssignSprite(sprite, "hotline_miami:Health:health_2");
            }
            else if (player.health == 1)
            {
                AssignSprite(sprite, "hotline_miami:Health:health_1");
            }
            else if (player.health == 0)
            {
                if (player.end)
                {
                    player.end = false;
                    auto& engine = Engine::Instance();
                    auto& reg = engine.Registry();
                    auto entity = reg.create();

                    auto& transform = reg.emplace<Transform>(entity);
                    transform.position.x = p_transform.position.x;
                    transform.position.y = p_transform.position.y;
                    transform.position.z = 1.f;
                    auto& sprite = reg.emplace<Sprite>(entity);

                    AssignSprite(sprite, "hotline_miami:EndScreen:bad");
                    sprite.size.x = 800;
                    sprite.size.y = 600;
                   
                }
                AssignSprite(sprite, "hotline_miami:Health:health_0");
            }
            sprite.size.x = 160.f;
            sprite.size.y = 32.f;
            
            t.position.x = p_transform.position.x - 300.f;
            t.position.y = p_transform.position.y - 250.f;
        }
    }
}
