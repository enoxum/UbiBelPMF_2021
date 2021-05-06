#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace robo_game
{
    struct ControllerMapping
    {
        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;
        EDaggerKeyboard upKey = EDaggerKeyboard::KeyW;
        EDaggerKeyboard downKey = EDaggerKeyboard::KeyS;


        Vector4 input{ 0, 0, 0, 0};
    };

    class SelectedTileInputSystem
        : public System
    {
    public:
        inline String SystemName() { return "Selected Tile Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}