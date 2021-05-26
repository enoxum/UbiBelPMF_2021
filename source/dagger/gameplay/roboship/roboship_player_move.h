#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace roboship
{
    struct ControllerMappingPlayer
    {
        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyLeft;
        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyRight;
        EDaggerKeyboard jumpKey = EDaggerKeyboard::KeyG;
        EDaggerKeyboard fightKey = EDaggerKeyboard::KeyF;

        Vector2 input{ 0, 0 };
    };

    struct RoboshipPlayer
    {
        Float32 speed { 1 };
    };

    class RoboshipPlayerInputSystem
        : public System
    {
    public:
        inline String SystemName() { return "Roboship Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}
