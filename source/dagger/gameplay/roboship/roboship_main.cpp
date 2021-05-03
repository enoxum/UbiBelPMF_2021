#include "gameplay/roboship/roboship_main.h"
#include "gameplay/roboship/roboship_controller.h"

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

#include "tools/diagnostics.h"

using namespace dagger;
using namespace roboship;

void Roboship::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();


    engine.AddSystem<RoboshipControllerSystem>();
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

        chr.sprite.scale = { 0.1f, 0.1f };
        chr.sprite.position = { position_, 0.0f };
        //chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "robot:IDLE:Idle (1)");
        AnimatorPlay(chr.animator, "robot:IDLE");

        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 50;

        return chr;
    }
};

void RoboshipSetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

void RoboshipCreateBackdrop()
{
    auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();

    // Create terrain 
    {
        auto back = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(back);

        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 200, 30 };
        sprite.scale = { 10, 1 };
        sprite.position = { 0, -135, 1 };
    }
    

    /* Put background image */ {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "robot:BACKGROUND:background1");
        sprite.scale.x = 0.25f;
        sprite.scale.y = 0.25f;
        sprite.position.x = 0;
        sprite.position.y = 5;
        sprite.position.z = 10;
    }

    /*
    {
        auto ui = reg.create();
        auto& text = reg.emplace<Text>(ui);
        text.spacing = 0.6f;
        text.Set("pixel-font", "Roboship game");
    }
    */
}


void Roboship::WorldSetup()
{
    RoboshipSetCamera();
    RoboshipCreateBackdrop();

    auto sndChar = RCharacter::Create("Arrows", { 1, 0, 0 }, { -120, -95 });
    //Engine::Registry().emplace<CameraFollowFocus>(sndChar.entity);
}

