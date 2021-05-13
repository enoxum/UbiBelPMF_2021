#pragma once
#include "core/system.h"
#include "core/core.h"
#include "tools/toolmenu.h"

using namespace dagger;

namespace atonement {
    struct SelectionMapping
    {
        EDaggerKeyboard upKey = EDaggerKeyboard::KeyUp;
        EDaggerKeyboard downKey = EDaggerKeyboard::KeyDown;


        Vector4 input{ 0, 0, 0, 0};
    };


    struct AtonementStartMenu : public System {
        private:
         void OnKeyboardEvent(KeyboardEvent kEvent_);
        public:
         void SpinUp() override;
         void Run() override;
         void WindDown() override;
         String SystemName();
    };
};