#include "tank_movement.h"
#include "tank_bullet.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"
#include "tank.h"

#include <iostream>
using namespace std;

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

void CreateTankBullet(float tileSize_, ColorRGBA color_, Vector3 speed_, Vector3 pos_, String desc)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "bullet");
    sprite.size = Vector2(1, 1) * tileSize_;

    sprite.color = color_;

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos_;
    transform.position.z = pos_.z;
    auto& ball = reg.emplace<TankBullet>(entity);
    ball.speed = speed_ * tileSize_;
    ball.tank = desc;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = tileSize_;
    col.size.y = tileSize_;
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

            if (kEvent_.key == ctrl_.fire_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed)) {
                ctrl_.fire = 1;
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
        Float32 rad;
        
        if (ctrl.move){
        	if (ctrl.move == 1.0 && tank.speed > 0.0 || ctrl.move == -1.0 && tank.speed < 0.0){
        		tank.rotationSpeed = 45.0;
        		tank.driftAngle = tank.angle;
        		tank.direction = ctrl.move;
        		tank.speed = tank.speed + ctrl.move * tank.acceleration * Engine::DeltaTime();
        	}
        	else{
        		tank.rotationSpeed = 60.0;
        		tank.speed = tank.speed + 1.5 * ctrl.move * tank.acceleration * Engine::DeltaTime();
        	}
        	if (tank.speed > tank.maxSpeed)
        		tank.speed = tank.maxSpeed;
        	if (tank.speed < tank.minSpeed)
        		tank.speed = tank.minSpeed;
        }
        else {
        	tank.rotationSpeed = 60.0;
        	tank.driftAngle = tank.angle;
        	tank.speed = tank.speed - 1.2 * tank.direction * tank.acceleration * Engine::DeltaTime();
        	if (tank.direction == 1.0 && tank.speed < 0.0 || tank.direction == -1.0 && tank.speed > 0.0){
        		tank.direction = 0.0;
        		tank.speed = 0.0;
        	}
        }

        if (col.colided)
        {
        	auto viewBullet = Engine::Registry().view<TankBullet, Transform, SimpleCollision>();

            bool tankDestroyed = false;

            for (auto entityBullet : viewBullet){
                if (col.colidedWith == entityBullet){
                    tank.health -= 25;   
                }
                if(tank.health <= 0){
                    Engine::Registry().destroy(entity);
                    tankDestroyed = true; 
                }
            }
            
            if(!tankDestroyed){
            	
            	tank.angle = tank.last_angle;
            	s.rotation = {-90.0f + tank.angle};
        		rad = tank.angle * PI / 180.0f;
        		col.angle = rad;
        		
        		tank.speed = 0.0;
        		tank.direction = 0.0;
                t.position = tank.pos;
                col.colided = false;
            }
        }
        else 
        {
        	tank.last_angle = tank.angle;
        	tank.angle += ctrl.rotation * Engine::DeltaTime() * tank.rotationSpeed;
        	s.rotation = {-90.0f + tank.angle};
        	rad = tank.angle * PI / 180.0f;
        	col.angle = rad;
        	
        	tank.pos = t.position;
        	if (ctrl.move == 0 || ctrl.move == tank.direction){
        		t.position.x += cos(rad) * tank.speed;
        		t.position.y += sin(rad) * tank.speed;
        	}
        	else{
        		t.position.x += cos(tank.driftAngle * PI / 180.0f) * tank.speed;
        		t.position.y += sin(tank.driftAngle * PI / 180.0f) * tank.speed;
        	}
        }
        
        if (ctrl.fire) {
            ctrl.fire = 0;
            if(tank.description == "tank1" && tank1_num_bullets < 5){
                tank1_num_bullets += 1;
                CreateTankBullet(
                    20, 
                    ColorRGBA(1, 1, 1, 1), 
                    { sin(-s.rotation * PI / 180.0f) * 10 ,
                      cos(-s.rotation * PI / 180.0f) * 10 ,
                      0 
                    },

                    {   tank.pos.x + 42 * cos(rad),
                        tank.pos.y + 42 * sin(rad),
                      0 
                    },
                    tank.description
                );
            }
            if(tank.description == "tank2" && tank2_num_bullets < 5){
                tank2_num_bullets += 1;
                CreateTankBullet(
                    20, 
                    ColorRGBA(1, 1, 1, 1), 
                    { sin(-s.rotation * PI / 180.0f) * 10 ,
                      cos(-s.rotation * PI / 180.0f) * 10 ,
                      0 
                    },

                    {   tank.pos.x + 42 * cos(rad),
                        tank.pos.y + 42 * sin(rad),
                      0 
                    },
                    tank.description
                );
            }
        }
    }
}
