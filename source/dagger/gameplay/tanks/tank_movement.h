#pragma once

#include "core/system.h"
#include "core/core.h"

#define PI 3.14159265

using namespace dagger;

namespace tanks
{
    struct ControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard left_key;
        EDaggerKeyboard right_key;

        Float32 move{0};
        Float32 rotation{0};
    };
    
    struct Tank
    {
        Float32 speed;
        Float32 angle;
    };

    class TankMovement
        : public System
    {

    public:
        inline String SystemName() { return "Tank Movement System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerOneMovement(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
            controllerMapping_.left_key = EDaggerKeyboard::KeyA;
            controllerMapping_.right_key = EDaggerKeyboard::KeyD;
        }

        static void SetupPlayerTwoMovement(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyUp;
            controllerMapping_.down_key = EDaggerKeyboard::KeyDown;
            controllerMapping_.left_key = EDaggerKeyboard::KeyLeft;
            controllerMapping_.right_key = EDaggerKeyboard::KeyRight;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}
