#include "hotline_miami_tools.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include <algorithm>
#include <execution>

using namespace dagger;
using namespace hotline_miami;

void HotlineMiamiDeleteEntitySystem::Run()
{
    auto view = Engine::Registry().view<HotlineMiamiDeleteEntity>();
    for (auto entity : view)
    {
        Engine::Registry().destroy(entity);
    }
}