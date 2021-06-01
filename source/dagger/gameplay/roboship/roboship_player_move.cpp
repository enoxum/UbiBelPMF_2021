#include "roboship_player_move.h"
#include "gameplay/roboship/roboship_createbackdrop.h"
#include "gameplay/roboship/fightEnemy.h"
#include "gameplay/roboship/roboship_camera_focus.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

#include <math.h>

const int TERRAIN_HEIGHT = 30;

using namespace dagger;
using namespace roboship;
using namespace fightEnemy;

static int count = 0;
static int iteration = 1;
static bool jumpActive = false;
static bool prepareFightMode = false;
static bool jumpAllow = false;
static bool jumpAllowed = false;
static bool atomicJump = false;
static bool atomicFight = false;
static bool stop = false;
static int win = 0;
static bool winIter = true;
static bool spaceshipFly = false;
static bool backJump = false;

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
    
    auto view = Engine::Registry().view<Sprite, ControllerMappingPlayer, FightEnded, Animator, RoboshipPlayer, RGameOver>();
    auto view2 = Engine::Registry().view<MoveWithRobot,Transform>();

    for (auto entity : view)
    {
        
        auto& sprite = view.get<Sprite>(entity);
        auto& ctrl = view.get<ControllerMappingPlayer>(entity);
        auto& roboshipPlayer = view.get<RoboshipPlayer>(entity);
        auto& animator = view.get<Animator>(entity);
        
        auto& fightOff = view.get<FightEnded>(entity);
        auto& gameOver = view.get<RGameOver>(entity);

        if (spaceshipFly) {
            auto spaceshipView = Engine::Registry().view<Sprite, RSpaceShip>();

            for (auto ss : spaceshipView) {
                auto& spaceshipSprite = spaceshipView.get<Sprite>(ss);
                spaceshipSprite.position.x += roboshipPlayer.speed * Engine::DeltaTime();
                spaceshipSprite.position.y += roboshipPlayer.speed * Engine::DeltaTime();
            }
        }
        else if (gameOver._gameover && !stop) {
            auto view3 = Engine::Registry().view<RBackDrop>();

            Engine::Dispatcher().trigger<RFightModeOff>();
            
            AnimatorPlay(animator, "robot:IDLE");
            stop = true;
            {
                auto entity = Engine::Registry().create();
                auto& text = Engine::Registry().emplace<Text>(entity);
                text.spacing = 0.6f;

                text.Set("pixel-font", "GAME OVER");
            }
            {
                auto entity = Engine::Registry().create();
                auto& bckg = Engine::Registry().get_or_emplace<Sprite>(entity);

                AssignSprite(bckg, "robot:BACKGROUND:blackground");

                bckg.scale.x = 10.f;
                bckg.scale.y = 10.f;
                bckg.position.x = sprite.position.x;
                bckg.position.y = 5;
                bckg.position.z = 0;
            }
        }
        else {
            if (fightOff._fightmodeoff_true) {
                if (winIter && win < 3) {
                    win++;
                }
                winIter = false;

                Engine::Dispatcher().trigger<REnemyDies>();
                
                AnimatorPlay(animator, "robot:JUMPMELEE");

                atomicFight = true;
                
                if (sprite.position.y + 223 <= 2 && !jumpActive && jumpAllow)
                    jumpActive = true;
                else if (sprite.position.y < -190 && jumpActive) {
                    sprite.position.y += roboshipPlayer.speed / 2.5 * Engine::DeltaTime();
                    sprite.position.x += roboshipPlayer.speed * 2 * Engine::DeltaTime();
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
                    sprite.position.y -= roboshipPlayer.speed / 3 * Engine::DeltaTime();
                    sprite.position.x += roboshipPlayer.speed * 2 * Engine::DeltaTime();
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
                    fightOff._fightmodeoff_true = false;
                    winIter = true;
                    Engine::Dispatcher().trigger<RFightModeOff>();
                }               
            }
            else if (EPSILON_ZERO(ctrl.input.x) && !atomicJump && !atomicFight)
            {
                if (!prepareFightMode)
                    AnimatorPlay(animator, "robot:IDLE");
                else if (prepareFightMode)
                    AnimatorPlay(animator, "robot:MELEE");
            }
            else if (!backJump && ((ctrl.input.x == 2 && jumpAllowed) || atomicJump) && !stop) {
                AnimatorPlay(animator, "robot:JUMP");

                atomicJump = true;

                if (sprite.position.y + 223 <= 2 && !jumpActive && jumpAllow)
                    jumpActive = true;
                else if (sprite.position.y < -30 && jumpActive) {
                    sprite.position.y += roboshipPlayer.speed * Engine::DeltaTime();
                    sprite.position.x += (roboshipPlayer.speed / 1.15) * Engine::DeltaTime();
                    for (auto e : view2) {
                        auto& t = Engine::Registry().get<Transform>(e);
                        t.position.x += roboshipPlayer.speed / 1.15 * Engine::DeltaTime();
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
                        t.position.x += roboshipPlayer.speed / 1.15 * Engine::DeltaTime();
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
            else if (backJump && ((ctrl.input.x == 2 && jumpAllowed) || atomicJump) && !stop) {
                AnimatorPlay(animator, "robot:JUMP");

                atomicJump = true;

                if (sprite.position.y + 223 <= 2 && !jumpActive && jumpAllow)
                    jumpActive = true;
                else if (sprite.position.y < -30 && jumpActive) {
                    sprite.position.y += roboshipPlayer.speed * Engine::DeltaTime();
                    sprite.position.x -= (roboshipPlayer.speed / 1.15) * Engine::DeltaTime();
                    for (auto e : view2) {
                        auto& t = Engine::Registry().get<Transform>(e);
                        t.position.x -= roboshipPlayer.speed / 1.15 * Engine::DeltaTime();
                    }
                }
                else if (sprite.position.y + 30 <= 2 && jumpActive) {
                    jumpActive = false;
                    jumpAllow = false;
                }
                else if (sprite.position.y > -223 && !jumpActive) {
                    sprite.position.y -= roboshipPlayer.speed * Engine::DeltaTime();
                    sprite.position.x -= (roboshipPlayer.speed / 1.15) * Engine::DeltaTime();
                    for (auto e : view2) {
                        auto& t = Engine::Registry().get<Transform>(e);
                        t.position.x -= roboshipPlayer.speed / 1.15 * Engine::DeltaTime();
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
                jumpAllowed = false;
                Engine::Dispatcher().trigger<RFightModeOn>();
            }
            else if (((ctrl.input.x == 1 || (ctrl.input.x == -1 && sprite.position.x > 0)) && !prepareFightMode) && !stop) {

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

                if (ctrl.input.x != -1 && fmod(sprite.position.x + 160, 800) <= 6 && sprite.position.x + 100 > 6 && !jumpAllowed && !(abs(sprite.position.x + 160 - 800 * 6) <= 6)) {
                    backJump = false;
                    Engine::Dispatcher().trigger<RPrepareFightModeOn>();
                    prepareFightMode = true;
                    AnimatorPlay(animator, "robot:MELEE");
                    jumpAllowed = true;
                }
                else if (ctrl.input.x == -1 && fmod(sprite.position.x - 160, 800) <= 6 && sprite.position.x + 100 > 6 && !jumpAllowed ) {
                    backJump = true;
                    Engine::Dispatcher().trigger<RPrepareFightModeOn>();
                    prepareFightMode = true;
                    AnimatorPlay(animator, "robot:MELEE");
                    jumpAllowed = true;
                }
                else if (abs(sprite.position.x - 800 * 6) <= 6 && win == 3) {
                    auto robotView = Engine::Registry().view<Sprite, RoboshipPlayer>();

                    for (auto r : robotView) {
                        auto& robotSprite = robotView.get<Sprite>(r);
                        robotSprite.position.y = -800;
                    }
                    spaceshipFly = true;
                    stop = true;
                    auto ent = Engine::Registry().create();
                    auto& text = Engine::Registry().emplace<Text>(ent);
                    text.spacing = 0.6f;

                    text.Set("pixel-font", "Congratulations!");
                }

            }

            if (abs(sprite.position.x - 800) <= 2 && iteration == 1)
            {
                iteration++;
                count += 1600;

                {
                    auto entity = Engine::Registry().create();
                    auto& sprite = Engine::Registry().get_or_emplace<Sprite>(entity);

                    AssignSprite(sprite, "robot:BACKGROUND:background2_infinite");

                    sprite.position.x = count;
                    sprite.position.y = 5;
                    sprite.position.z = 10;
                }
                {
                    auto entity = Engine::Registry().create();
                    auto& sprite = Engine::Registry().get_or_emplace<Sprite>(entity);

                    AssignSprite(sprite, "EmptyWhitePixel");
                    sprite.color = { 0, 0, 0, 1 };
                    sprite.size = { 2000, TERRAIN_HEIGHT };
                    sprite.scale = { 10, 1 };
                    sprite.position = { sprite.position.x, -(600 - TERRAIN_HEIGHT) / 2, 1 };
                }
            }
            else if (abs((sprite.position.x - 800) - (iteration - 1) * 2000) - 400 * (iteration - 1) <= 2 && iteration > 1) {
                iteration++;
                count += 1600;

                {
                    auto entity = Engine::Registry().create();
                    auto& sprite = Engine::Registry().get_or_emplace<Sprite>(entity);

                    AssignSprite(sprite, "robot:BACKGROUND:background2_infinite");

                    /*sprite.scale.x = 0.5f;
                    sprite.scale.y = 0.5f;*/
                    sprite.position.x = count;
                    sprite.position.y = 5;
                    sprite.position.z = 10;
                }

                {
                    auto entity = Engine::Registry().create();
                    auto& sprite = Engine::Registry().get_or_emplace<Sprite>(entity);

                    AssignSprite(sprite, "EmptyWhitePixel");
                    sprite.color = { 0, 0, 0, 1 };
                    sprite.size = { 2000, TERRAIN_HEIGHT };
                    sprite.scale = { 10, 1 };
                    sprite.position = { sprite.position.x, -(600 - TERRAIN_HEIGHT) / 2, 1 };
                }
            }
        }
    
    }
}