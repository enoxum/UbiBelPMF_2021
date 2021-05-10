#include "pandemic_player_input.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace pandemic_shop;

//void PandemicShopPlayerInputSystem::Run()
//{
//    auto view = Engine::Registry().view<Transform, ControllerMapping>();
//    for (auto entity : view)
//    {
//        auto &t = view.get<Transform>(entity);
//        auto &ctrl = view.get<ControllerMapping>(entity);
//
//        t.position.y += ctrl.input.y * s_PlayerSpeed * Engine::DeltaTime();
//        t.position.x += ctrl.input.x * s_PlayerSpeed * Engine::DeltaTime();
//
//        if (t.position.y > s_BorderUp)
//        {
//            t.position.y = s_BorderUp;
//        }
//
//        if (t.position.y < s_BorderDown)
//        {
//            t.position.y = s_BorderDown;
//        }
//
//        if (t.position.x > s_BorderRight)
//        {
//            t.position.x = s_BorderRight;
//        }
//
//        if (t.position.x < s_BorderLeft)
//        {
//            t.position.x = s_BorderLeft;
//        }
//    }
//}
