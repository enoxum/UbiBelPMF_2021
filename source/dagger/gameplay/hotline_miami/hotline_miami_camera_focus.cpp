#include "hotline_miami_camera_focus.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

#include "core/game/transforms.h"

using namespace dagger;
using namespace hotline_miami;

void HotlineMiamiCameraFollowSystem::Run()
{
    auto* camera = Engine::GetDefaultResource<Camera>();

    auto view = Engine::Registry().view<Transform, HotlineMiamiCameraFollowFocus>();

    for (auto entity : view) {
    
        auto& t = view.get<Transform>(entity);
        Vector2 center{ t.position.x, t.position.y};
        camera->position = Vector3{ glm::mix((Vector2)camera->position, center, 0.5f), 0.0f };

    }
}