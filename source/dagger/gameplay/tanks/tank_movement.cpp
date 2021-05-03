#include "tank_movement.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"
#include "tanks_main.h"

#include <cmath>

using namespace dagger;
using namespace tanks;

void TankMovement::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&TankMovement::OnKeyboardEvent>(this);
}

void TankMovement::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&TankMovement::OnKeyboardEvent>(this);
}

void TankMovement::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        
        if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
        	ctrl_.rotation = 1;
        }
        else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.rotation > 0)
        {
        	ctrl_.rotation = 0;
        }
        else if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
        	ctrl_.rotation = -1;
        }
        else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.rotation < 0)
        {
        	ctrl_.rotation = 0;
        }
        
        if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
        	ctrl_.move = 1;
        }
        else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.move > 0)
        {
        	ctrl_.move = 0;
        }
        else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
        	ctrl_.move = -1;
        }
        else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.move < 0)
        {
        	ctrl_.move = 0;
        }
    });
}

void TankMovement::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping, Tank, Sprite, SimpleCollision>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);
        auto &tank = view.get<Tank>(entity);
        auto &s = view.get<Sprite>(entity);
        auto &col = view.get<SimpleCollision>(entity);
        
        tank.angle += ctrl.rotation * Engine::DeltaTime() * 30.0;
        s.rotation = {-90.0f + tank.angle};
        
        if (col.colided)
        {
        	// kad se dodaju metkovi ovo vrv treba izmeniti
        	t.position = tank.pos;
            col.colided = false;
        }
        else 
        {
        	tank.pos = t.position;
        	t.position.x += cos(tank.angle * PI / 180.0f) * ctrl.move * tank.speed * Engine::DeltaTime();
        	t.position.y += sin(tank.angle * PI / 180.0f) * ctrl.move * tank.speed * Engine::DeltaTime();
        }
    }
}
