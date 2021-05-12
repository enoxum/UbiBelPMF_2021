#include "roboship_camera_focus.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

using namespace dagger;
using namespace roboship;

void RCameraFollowSystem::Run()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->position = { 0, 0, 0 };
    auto camPosition = (Vector2)camera->position;

    Vector2 center{};
    UInt32 count{ 0 };

    Engine::Registry().view<RCameraFollowFocus, Sprite>().each([&](const RCameraFollowFocus& focus_, const Sprite& sprite_)
        {
            for (UInt32 i = 0; i < focus_.weight; i++)
            {
                center += (Vector2)sprite_.position;
            }
            count += focus_.weight;
        });

    if (count > 0)
    {
        //center /= count;
        camera->position = Vector3{ glm::mix((Vector2)camera->position, center, 1.0f), 0.0f };
        camera->position.y = 0;
    }
}
