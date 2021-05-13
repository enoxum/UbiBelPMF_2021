#include "hotline_miami_player_input.h"
#include "hotline_miami_player.h"
#include "hotline_miami_projectile.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

using namespace dagger;
using namespace hotline_miami;


void HotlineMiamiPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&HotlineMiamiPlayerInputSystem::OnKeyboardEvent>(this);
}

void HotlineMiamiPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&HotlineMiamiPlayerInputSystem::OnKeyboardEvent>(this);
}

void HotlineMiamiPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
        {
            // moving
            if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.y = 1;
            }
            else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.y = -1;
            }
            else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.x = -1;
            }
            else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = 1;
            }
            else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
            }

            // looking
            if (kEvent_.key == ctrl_.look_up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.look_direction = TOP;
            }
            else if (kEvent_.key == ctrl_.look_down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.look_direction = DOWN;
            }
            else if (kEvent_.key == ctrl_.look_left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.look_direction = LEFT;
            }
            else if (kEvent_.key == ctrl_.look_right_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.look_direction = RIGHT;
            }

            // shooting
            if (kEvent_.key == ctrl_.shoot_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.shoot = true;
            }

        });
}

void HotlineMiamiPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping, HotlineMiamiPlayer, Sprite, SimpleCollision>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& player = view.get<HotlineMiamiPlayer>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        auto& sprite = view.get<Sprite>(entity);
    
        if (ctrl.shoot)
        {
            // make a projectile
            {
                constexpr float tileSize = 15.f;

                auto& engine = Engine::Instance();
                auto& reg = engine.Registry();
                auto entity = reg.create();

                auto& collision = reg.emplace<SimpleCollision>(entity);
                collision.size.x = tileSize;
                collision.size.y = tileSize;

                auto& transform = reg.emplace<Transform>(entity);
                if (ctrl.look_direction == DOWN)
                {
                    transform.position = t.position + Vector3(0, -sprite.size.y, 0);
                }
                // TODO treba lepse racunati gde postaviti metak inace ce da koliduje sa playerom i puca
                else if (ctrl.look_direction == TOP)
                {                             
                    transform.position = t.position + Vector3(0, tileSize * 2, 0);
                }
                else if (ctrl.look_direction == LEFT)
                {
                    transform.position = t.position + Vector3(-tileSize * 2, 0, 0);
                }
                else if (ctrl.look_direction == RIGHT)
                {
                    transform.position = t.position + Vector3(sprite.size.x, 0, 0);
                }

                auto& sprite = reg.emplace<Sprite>(entity);
                AssignSprite(sprite, "hotline_miami:Projectile:pistol_gun_1");
                sprite.rotation = ctrl.look_direction * 90.f;
                sprite.size.x = tileSize;
                sprite.size.y = tileSize;

                auto& projectile = reg.emplace<HotlineMiamiProjectile>(entity);
                HotlineMiamiProjectileSystem::SetupProjectileStats(projectile, ctrl.look_direction);
            }
            ctrl.shoot = false;
        }

        t.position.y += ctrl.input.y * player.player_speed * Engine::DeltaTime();
        t.position.x += ctrl.input.x * player.player_speed * Engine::DeltaTime();

        sprite.rotation = ctrl.look_direction * 90.f;

    }   
}
