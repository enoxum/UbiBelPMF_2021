#include "parallax.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

using namespace dagger;
using namespace atonement;

void ParallaxSystem::Run()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    auto camPosition = (Vector2)camera->position;

    Engine::Registry().view<Parallax, Sprite>().each([&](Parallax& parallax_, Sprite& sprite_)
        {
            auto delta = (parallax_.lastCameraPosition - camPosition) * parallax_.strength;
            sprite_.position.x += delta.x;
            sprite_.position.y = camPosition.y;
            parallax_.lastCameraPosition = camPosition;
        });

    if (camPosition.x > rightmost.position.x) {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& parallax = reg.get_or_emplace<Parallax>(entity);
        parallax.lastCameraPosition = camera->position;
        parallax.strength = 0.00001;

        AssignSprite(sprite, "MossyBackground:scrolling_bg1");
        sprite.position = { rightmost.position.x + rightmost.size.x, -225, 100 };
        sprite.scale = rightmost.scale;
        sprite.scale.x *= -1;

        rightmost = sprite;
    }

    if (camPosition.x < leftmost.position.x) {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& parallax = reg.get_or_emplace<Parallax>(entity);
        parallax.lastCameraPosition = camera->position;
        parallax.strength = 0.00001;

        AssignSprite(sprite, "MossyBackground:scrolling_bg1");
        sprite.position = { leftmost.position.x - leftmost.size.x, -225, 100 };
        sprite.scale = leftmost.scale;
        sprite.scale.x *= -1;

        leftmost = sprite;
    }
}

void ParallaxSystem::SpinUp()
{
    auto* camera = Engine::GetDefaultResource<Camera>();

    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.get_or_emplace<Sprite>(entity);
    auto& parallax = reg.get_or_emplace<Parallax>(entity);
    parallax.lastCameraPosition = camera->position;
    parallax.strength = 0.00001;

    AssignSprite(sprite, "MossyBackground:scrolling_bg1");
    sprite.position = { 600, -225, 99 };

    leftmost = sprite;
    rightmost = sprite;
}