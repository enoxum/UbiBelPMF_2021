#include "tank_bullet.h"
#include "tank.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include <execution>

#include <iostream>

using namespace dagger;
using namespace tanks;

void TankBulletSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<TankBullet, Transform, SimpleCollision>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ball = view.get<TankBullet>(entity);
        auto& col = view.get<SimpleCollision>(entity);


        if (col.colided)
        {

            //std::cout <<"\n\n"<<"Milica Sudar " << t.position.x<<" "<< t.position.y<<" "<<t.position.z<< "\n\n";
            if(!ball.special_bullet){
                if(ball.tank == "tank1"){
                    tank1_num_bullets -= 1;
                }
                else if (ball.tank == "tank2"){
                    tank2_num_bullets -= 1;
                }
                Engine::Registry().destroy(entity);
            }
            else{
                if (Engine::Registry().valid(col.colidedWith))
                {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);

                    Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);
                    ball.number_of_collisions += 1;
                    do
                    {
                        // get back for 1 frame 
                        Float32 dt = Engine::DeltaTime();
                        if (std::abs(collisionSides.x) > 0)
                        {
                            t.position.x -= (ball.speed.x * dt);
                        }

                        if (std::abs(collisionSides.y) > 0)
                        {
                            t.position.y -= (ball.speed.y * dt);
                        }

                    } while (col.IsCollided(t.position, collision, transform.position));

                    if (std::abs(collisionSides.x) > 0)
                    {
                        ball.speed.x *= -1;
                    }

                    if (std::abs(collisionSides.y) > 0)
                    {
                        ball.speed.y *= -1;
                    }

                    if(ball.number_of_collisions == 3){
                        if(ball.tank == "tank1"){
                            tank1_num_bullets -= 1;
                        }
                        else if (ball.tank == "tank2"){
                            tank2_num_bullets -= 1;
                        }
                        Engine::Registry().destroy(entity);
                    }

                    auto viewTank = Engine::Registry().view<Tank, Transform, SimpleCollision>();

                    for (auto entityTank : viewTank){
                        if (col.colidedWith == entityTank){
                            if(ball.tank == "tank1"){
                                tank1_num_bullets -= 1;
                            }
                            else if (ball.tank == "tank2"){
                                tank2_num_bullets -= 1;
                            }
                            Engine::Registry().destroy(entity);    
                        }
                        
                    }

                }
            }
            


            col.colided = false;
        }
        else
        {
            t.position += (ball.speed * Engine::DeltaTime());
        }
    }
}
