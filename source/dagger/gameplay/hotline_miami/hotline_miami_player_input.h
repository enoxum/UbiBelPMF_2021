#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{
    enum DIRECTION { look_down, look_right, look_top, look_left };

    struct ControllerMapping
    {
        // moving
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard left_key;
        EDaggerKeyboard right_key;

        // shooting direction
        EDaggerKeyboard look_up_key;
        EDaggerKeyboard look_down_key;
        EDaggerKeyboard look_left_key;
        EDaggerKeyboard look_right_key;

        // shooting
        EDaggerKeyboard shoot_key;

        // moving input
        Vector2 input{ 0, 0 };

        DIRECTION look_direction;
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

            controllerMapping_.look_up_key = EDaggerKeyboard::KeyUp;
            controllerMapping_.look_down_key = EDaggerKeyboard::KeyDown;
            controllerMapping_.look_left_key = EDaggerKeyboard::KeyLeft;
            controllerMapping_.look_right_key = EDaggerKeyboard::KeyRight;

            controllerMapping_.shoot_key = EDaggerKeyboard::KeySpace;

            controllerMapping_.look_direction = look_down;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}