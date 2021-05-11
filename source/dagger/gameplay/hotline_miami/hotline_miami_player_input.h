#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{
    struct ControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard left_key;
        EDaggerKeyboard right_key;

        Vector2 input{ 0, 0 };
    };

    class HotlineMiamiPlayerInputSystem
        : public System
    {
    public:
        inline String SystemName() { return "HotlineMiami Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
            controllerMapping_.left_key = EDaggerKeyboard::KeyA;
            controllerMapping_.right_key = EDaggerKeyboard::KeyD;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}