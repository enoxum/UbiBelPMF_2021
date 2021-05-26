#include "intro_system.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"
#include "gameplay/brawler/systems/menu_main.h"

using namespace dagger;
using namespace brawler;

void IntroSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&IntroSystem::OnKeyboardEvent>(this);
    Engine::ToggleSystemsPause(true);

    auto& reg = Engine::Registry();

    {
        background = reg.create();
        auto& s = reg.emplace<Sprite>(background);
        AssignSprite(s, "EmptyWhitePixel");
        s.position = { 0.0f, 0.0f, 50.0f };
        s.color = {0.0f, 0.0f, 0.0f, 1.0f};
        s.scale = { 1280, 720 };
        s.UseAsUI();
    }

    {
        dagger = reg.create();
        auto& dagger_transform = reg.emplace<Transform>(dagger);
        dagger_transform.position = { 0, 500, 40.0f };
        auto& dagger_sprite = reg.emplace<Sprite>(dagger);
        AssignSprite(dagger_sprite, "logos:dagger");
        dagger_sprite.position = { 0.0f, 500.0f, 40.0f };
        dagger_sprite.scale = {0.035f, 0.035f};
        dagger_sprite.UseAsUI();
    }

    {
        curtain = reg.create();
        auto& s = reg.emplace<Sprite>(curtain);
        AssignSprite(s, "EmptyWhitePixel");
        s.position = { 0.0f, 0.0f, 30.0f };
        s.color = { 0.0f, 0.0f, 0.0f, 0.0f };
        s.scale = { 1280, 720 };
        s.UseAsUI();
    }
}

void IntroSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&IntroSystem::OnKeyboardEvent>(this);
}

void IntroSystem::Run()
{
    if(ubisoftDone)
        return;

    auto& reg = Engine::Registry();
    if(!animationDone)
    {
        auto& dagger_transform = reg.get<Transform>(dagger);
        dagger_transform.position.y -= 800.0f * Engine::DeltaTime();
        if(dagger_transform.position.y < -100) {
            dagger_transform.position.y = -100.0f;

            {
                ui_text = reg.create();
                auto& text = reg.emplace<Text>(ui_text);
                text.spacing = 0.6f;
                text.letterSize = { 37.0f, 47.0f };
                text.Set("pixel-font", "Powered By Dagger", { 0, 200, 40.0f }, true);
            }
            animationDone = true;
        }
    }
    else
    {
        duration += Engine::DeltaTime();
        if (duration >= 2.0f && duration < 3.0f)
        {
            auto& s = reg.get<Sprite>(curtain);
            s.color.a = 1.0f - (3.0f - duration) / 1.0f;
        }
        else if (duration >= 3.0f && duration < 4.0f)
        {
            if (!daggerDone)
            {
                auto& backgroundSprite = reg.get<Sprite>(background);
                backgroundSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };

                auto& daggerTransform = reg.get<Transform>(dagger);
                daggerTransform.position = { 0.0f, 0.0f, 40.0f };
                auto& daggerSprite = reg.get<Sprite>(dagger);
                AssignSprite(daggerSprite, "logos:ubisoft_belgrade");
                daggerSprite.scale = { 1.0f, 1.0f };
                auto& text = reg.get<Text>(ui_text);
                text.Set("pixel-font", "");
            }
            daggerDone = true;
            auto& s = reg.get<Sprite>(curtain);
            s.color.a = (4.0f - duration) / 1.0f;
        }
        else if (duration >= 7.0f && duration < 8.0f)
        {
            auto& s = reg.get<Sprite>(curtain);
            s.color.a = 1.0f - (8.0f - duration) / 1.0f;
        }
        else if (duration >= 8.0f)
        {
            reg.destroy(background);
            reg.destroy(dagger);
            reg.destroy(curtain);
            reg.destroy(ui_text);
            ubisoftDone = true;
            Engine::GetDefaultResource<MenuMain>()->Toggle();
        }
    }
}

void IntroSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    if (ubisoftDone)
        return;

    if (kEvent_.action == EDaggerInputState::Released)
    {
        auto& reg = Engine::Registry();
        reg.destroy(background);
        reg.destroy(dagger);
        if (reg.valid(ui_text))
        {
            auto& text = reg.get<Text>(ui_text);
            text.Set("pixel-font", "");
            reg.destroy(ui_text);
        }
        reg.destroy(curtain);
        ubisoftDone = true;
        Engine::GetDefaultResource<MenuMain>()->Toggle();
    }
}