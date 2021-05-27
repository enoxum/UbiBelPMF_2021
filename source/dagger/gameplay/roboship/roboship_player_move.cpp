#include "roboship_player_move.h"
#include "gameplay/roboship/roboship_createbackdrop.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

#include <math.h>

using namespace dagger;
using namespace roboship;

static int count = 0;
static int iteration = 1;
static bool jumpActive = false;
static bool prepareFightMode = false;
static bool jumpAllow = false;
static bool jumpAllowed = false;
static bool atomicJump = false;
static bool atomicFight = false;
static bool stop = false;

void RoboshipPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&RoboshipPlayerInputSystem::OnKeyboardEvent>(this);
}

void RoboshipPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&RoboshipPlayerInputSystem::OnKeyboardEvent>(this);
}

void RoboshipPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMappingPlayer>().each([&](ControllerMappingPlayer& ctrl_)
        {
            if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.x = -1;
            }
            else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = 1;
            }
            else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.jumpKey && ( kEvent_.action == EDaggerInputState::Released))
            {
                ctrl_.input.x = 2;
            }
            else if (kEvent_.key == ctrl_.fightKey && (kEvent_.action == EDaggerInputState::Released))
            {
                ctrl_.input.x = 3;
            }
            else if (kEvent_.key == ctrl_.jumpKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
            }
        });
}


void RoboshipPlayerInputSystem::Run()
{
    
    auto view = Engine::Registry().view<Sprite, ControllerMappingPlayer, Animator, RoboshipPlayer>();
    auto view2 = Engine::Registry().view<MoveWithRobot,Transform>();

    for (auto entity : view)
    {

        auto& animator = view.get<Animator>(entity);

        auto& sprite = view.get<Sprite>(entity);
        auto& ctrl = view.get<ControllerMappingPlayer>(entity);
        auto& roboshipPlayer = view.get<RoboshipPlayer>(entity);

        if (EPSILON_ZERO(ctrl.input.x) && !atomicJump && !atomicFight)
        {
            if(!prepareFightMode)
                AnimatorPlay(animator, "robot:IDLE");
            else if(prepareFightMode)
                AnimatorPlay(animator, "robot:MELEE");
        }
        else if (((ctrl.input.x == 2 && jumpAllowed) || atomicJump) && !stop) {
            AnimatorPlay(animator, "robot:JUMP");

            atomicJump = true;

            if (sprite.position.y + 223 <= 2 && !jumpActive && jumpAllow)
                jumpActive = true;
            else if (sprite.position.y < -30 && jumpActive) {
                sprite.position.y += roboshipPlayer.speed * Engine::DeltaTime();
                sprite.position.x += (roboshipPlayer.speed / 1.15) * Engine::DeltaTime();
                for (auto e : view2) {
                    auto& t = Engine::Registry().get<Transform>(e);
                    t.position.x +=  roboshipPlayer.speed/1.15 * Engine::DeltaTime();
                }
            }
            else if (sprite.position.y + 30 <= 2 && jumpActive) {
                jumpActive = false;
                jumpAllow = false;
            }
            else if (sprite.position.y > -223 && !jumpActive) {
                sprite.position.y -= roboshipPlayer.speed * Engine::DeltaTime();
                sprite.position.x += (roboshipPlayer.speed / 1.15) * Engine::DeltaTime();
                for (auto e : view2) {
                    auto& t = Engine::Registry().get<Transform>(e);
                    t.position.x += roboshipPlayer.speed/1.15 * Engine::DeltaTime();
                }
            }
            else if (sprite.position.y + 223 <= 2 && !jumpActive && !jumpAllow) {
                ctrl.input.x = 0;
                prepareFightMode = false;
                jumpAllowed = false;
                atomicJump = false;
                Engine::Dispatcher().trigger<RPrepareFightModeOff>();
            }
        }
        else if (((ctrl.input.x == 3 && jumpAllowed) || atomicFight) && !stop) {
            Engine::Dispatcher().trigger<RFightModeOn>();
            AnimatorPlay(animator, "robot:JUMPMELEE");

            atomicFight = true;

            if (sprite.position.y + 223 <= 2 && !jumpActive && jumpAllow)
                jumpActive = true;
            else if (sprite.position.y < -190 && jumpActive) {
                sprite.position.y += roboshipPlayer.speed/2.5 * Engine::DeltaTime();
                sprite.position.x += roboshipPlayer.speed*2 * Engine::DeltaTime();
                for (auto e : view2) {
                    auto& t = Engine::Registry().get<Transform>(e);
                    t.position.x += 2 * roboshipPlayer.speed * Engine::DeltaTime();
                }
            }
            else if (sprite.position.y + 190 <= 2 && jumpActive) {
                jumpActive = false;
                jumpAllow = false;
            }
            else if (sprite.position.y > -223 && !jumpActive) {
                sprite.position.y -= roboshipPlayer.speed/3 * Engine::DeltaTime();
                sprite.position.x += roboshipPlayer.speed*2 * Engine::DeltaTime();
                for (auto e : view2) {
                    auto& t = Engine::Registry().get<Transform>(e);
                    t.position.x += 2 * roboshipPlayer.speed * Engine::DeltaTime();
                }
            }
            else if (sprite.position.y + 223 <= 2 && !jumpActive && !jumpAllow) {
                ctrl.input.x = 0;
                prepareFightMode = false;
                jumpAllowed = false;
                atomicFight = false;
                Engine::Dispatcher().trigger<RFightModeOff>();
            }
        }
        else if (((ctrl.input.x == 1 || ctrl.input.x == -1) && !prepareFightMode) && !stop) {

            if (!prepareFightMode) {
                for (auto e : view2) {
                    auto& t = Engine::Registry().get<Transform>(e);
                    t.position.x += ctrl.input.x * roboshipPlayer.speed * Engine::DeltaTime();
                }
                AnimatorPlay(animator, "robot:RUN");
                sprite.position.x += ctrl.input.x * roboshipPlayer.speed * Engine::DeltaTime();
                jumpAllow = true;
            }

            if (sprite.scale.x * ctrl.input.x < 0 && !prepareFightMode) {
                
                sprite.scale.x *= -1;
                Engine::Dispatcher().trigger<RChangeDirection>();

            }

            if (fmod(sprite.position.x + 160, 800) <= 6 && sprite.position.x + 100 > 6 && !jumpAllowed && !(abs(sprite.position.x+160-800*6) <= 6)) {
                Engine::Dispatcher().trigger<RPrepareFightModeOn>();
                prepareFightMode = true;
                AnimatorPlay(animator, "robot:MELEE");
                jumpAllowed = true;
            }
            else if(abs(sprite.position.x - 800 * 6) <= 6) {
                AnimatorPlay(animator, "robot:IDLE");
                stop = true;
            }

        }

        if (abs(sprite.position.x - 800) <= 2 && iteration == 1)
        {
            iteration++;
            count += 1600;
            RBackdrop::RoboshipCreateBackdrop(count, sprite.position.x);
        }
        else if (abs((sprite.position.x - 800) - (iteration - 1) * 2000) - 400 * (iteration - 1) <= 2 && iteration > 1) {
            iteration++;
            count += 1600;
            RBackdrop::RoboshipCreateBackdrop(count, sprite.position.x);
        }

    
    }
}