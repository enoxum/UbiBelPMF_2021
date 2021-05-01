#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace tanks
{
    struct ControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;

        Vector2 input{ 0, 0 };
    };

    class TankMovement
        : public System
    {

    public:

        static Float32 s_PlayerSpeed;

    public:
        inline String SystemName() { return "Tank Movement System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerOneMovement(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
        }

        static void SetupPlayerTwoMovement(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyUp;
            controllerMapping_.down_key = EDaggerKeyboard::KeyDown;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}
