#include "gameplay/roboship/roboship_main.h"
#include "gameplay/roboship/roboship_player_move.h";
#include "gameplay/roboship/roboship_camera_focus.h"
#include "gameplay/roboship/roboship_createbackdrop.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"

#include <chrono>
#include "tools/diagnostics.h"

const int TERRAIN_HEIGHT = 30;
static int brojac = 0;

using namespace dagger;
using namespace roboship;


void Roboship::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<RoboshipPlayerInputSystem>();
    engine.AddSystem<RCameraFollowSystem>();
}

REnemy* REnemy::Get(Entity entity)
{
    auto& reg = Engine::Registry();
    auto& sprite = reg.get_or_emplace<Sprite>(entity);
    auto& anim = reg.get_or_emplace<Animator>(entity);
    reg.get_or_emplace<EnemyMarker>(entity);

    return new REnemy{ entity, sprite, anim };
}

REnemy* REnemy::Create(
    ColorRGB color_ = { 1, 1, 1 },
    Vector2 position_ = { 0, 0 })
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    REnemy *chr = REnemy::Get(entity);
    chr->sprite.scale = { 0.15f, 0.15f };
    chr->sprite.position = { position_, 0.0f };
    //chr.sprite.color = { color_, 1.0f };

    AssignSprite(chr->sprite, "robot:ENEMIES:Robot1:01_Idle:idle_000");
    AnimatorPlay(chr->animator, "robot:ENEMIES:Robot1:01_Idle");

    return chr;
}

void REnemy::change_direction(RChangeDirection& ev)
{
    this->sprite.scale.x *= -1.0f;
}

void RoboshipSetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

void Roboship::WorldSetup()
{
    RoboshipSetCamera();
    RBackdrop::RoboshipCreateBackdrop(0, 0);

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    float zPos = 0.0f;

    // player

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        auto& animator = reg.get_or_emplace<Animator>(entity);

        AssignSprite(sprite, "robot:IDLE:Idle (1)");
        AnimatorPlay(animator, "robot:IDLE");

        sprite.scale = { 0.2f, 0.2f };
        sprite.position = { 0.0f, -223.0f, 0.0f };

        auto& roboshipPlayer = reg.emplace<RoboshipPlayer>(entity);
        roboshipPlayer.speed = 200;

        reg.emplace<ControllerMapping>(entity);
        Engine::Registry().emplace<RCameraFollowFocus>(entity);
    }

    Engine::Dispatcher().sink<RPrepareFightModeOn>().connect<&Roboship::ShowTextPrepareFightMode>(this);
    Engine::Dispatcher().sink<RPrepareFightModeOff>().connect<&Roboship::ShowTextPrepareFightMode>(this);


    int n_enemies = 5;
    for (int i = 0; i < n_enemies; i++)
    {
        REnemy* enemyChar = REnemy::Create({ 0, 1, 0 }, { (i+1) * 800, -200 });
        enemyChar->sprite.scale = { -0.15f, 0.15f };

        Engine::Dispatcher().sink<RChangeDirection>().connect<&Roboship::TurnRobots>(this);
    }
}

void Roboship::ShowTextPrepareFightMode(){
    Engine& engine = Engine::Instance();
    auto& reg = engine.Registry();
    

    if (brojac % 2 == 0) {
        auto ui = reg.create();

        auto& text = reg.emplace<Text>(ui);
        text.spacing = 0.6f;
        text.Set("pixel-font", "Fight(F) / Go on(G)");
    }
}

void Roboship::ClearTextPrepareFightMode(){

}

void Roboship::TurnRobots()
{
    Engine::Registry().view<Sprite, EnemyMarker>().each([](Sprite& sprite_, const EnemyMarker&)
        {
            sprite_.scale.x *= -1.0f;
        });
}
