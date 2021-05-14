#include "gameplay/roboship/roboship_main.h"
#include "gameplay/roboship/roboship_controller.h"
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

using namespace dagger;
using namespace roboship;

void Roboship::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<RoboshipControllerSystem>();
    engine.AddSystem<RCameraFollowSystem>();
}


struct RCharacter
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    RoboshipCharacter& character;

    static RCharacter Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<RoboshipCharacter>(entity);

        return RCharacter{ entity, sprite, anim, input, character };
    }

    static RCharacter Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();

        ATTACH_TO_FSM(RoboshipCharacterControllerFSM, entity);

        auto chr = RCharacter::Get(entity);

        chr.sprite.scale = { 0.2f, 0.2f };
        chr.sprite.position = { position_, 0.0f };
        //chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "robot:IDLE:Idle (1)");
        AnimatorPlay(chr.animator, "robot:IDLE");

        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 300;

        return chr;
    }
};

REnemy* REnemy::Get(Entity entity)
{
    auto& reg = Engine::Registry();
    auto& sprite = reg.get_or_emplace<Sprite>(entity);
    auto& anim = reg.get_or_emplace<Animator>(entity);

    return new REnemy{ entity, sprite, anim };
}

REnemy* REnemy::Create(
    ColorRGB color_ = { 1, 1, 1 },
    Vector2 position_ = { 0, 0 })
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    REnemy *chr = REnemy::Get(entity);

    chr->sprite.scale = { -0.15f, 0.15f };
    chr->sprite.position = { position_, 0.0f };
    //chr.sprite.color = { color_, 1.0f };

    AssignSprite(chr->sprite, "robot:ENEMIES:Robot1:01_Idle:idle_000");
    AnimatorPlay(chr->animator, "robot:ENEMIES:Robot1:01_Idle");

    return chr;
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

//void RoboshipCreateBackdrop()
//{
//    auto& reg = Engine::Registry();
//    auto* camera = Engine::GetDefaultResource<Camera>();
//
//    // Create terrain 
//    {
//        auto back = reg.create();
//        auto& sprite = reg.get_or_emplace<Sprite>(back);
//
//        AssignSprite(sprite, "EmptyWhitePixel");
//        sprite.color = { 0, 0, 0, 1 };
//        sprite.size = { 200, TERRAIN_HEIGHT };
//        sprite.scale = { 10, 1 };
//        sprite.position = { 0, -(camera->size.y - TERRAIN_HEIGHT) / 2, 1 };
//    }
//    
//
//    /* Put background image */
//    {
//        auto entity = reg.create();
//        auto& sprite = reg.get_or_emplace<Sprite>(entity);
//
//        AssignSprite(sprite, "robot:BACKGROUND:background2_infinite");
//        
//        /*sprite.scale.x = 0.5f;
//        sprite.scale.y = 0.5f;*/
//        sprite.position.x = 0;
//        sprite.position.y = 5;
//        sprite.position.z = 10;
//    }
//
//    /*
//    {
//        auto ui = reg.create();
//        auto& text = reg.emplace<Text>(ui);
//        text.spacing = 0.6f;
//        text.Set("pixel-font", "Roboship game");
//    }
//    */
//}


void Roboship::WorldSetup()
{
    RoboshipSetCamera();
    RBackdrop::RoboshipCreateBackdrop(0, 0);

    auto sndChar = RCharacter::Create("Arrows", { 1, 0, 0 }, {0, -223 });
    Engine::Registry().emplace<RCameraFollowFocus>(sndChar.entity);
    
    int n_enemies = 5;
    std::vector<REnemy*> enemies;
    for (int i = 0; i < n_enemies; i++)
    {
        REnemy* enemyChar = REnemy::Create({ 0, 1, 0 }, { (i+1) * 200, -200 });
        enemyChar->sprite.scale = { 0.15f, 0.15f };
        enemies.push_back(enemyChar);
    }

    sndChar.character.enemies = enemies;
}

