#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"

using namespace dagger;

namespace brawler
{

class IntroSystem : public System
{
    Float32 duration {0};

    Entity background;
    Entity dagger;
    Entity ui_text;
    Entity curtain;

    Bool animationDone {false};
    Bool daggerDone {false};
    Bool ubisoftDone{ false };

    String SystemName() override {
        return "Intro System";
    }

    void SpinUp() override;
    void WindDown() override;
    void Run() override;

    void OnKeyboardEvent(KeyboardEvent kEvent_);

public:
    Bool isDone() { return ubisoftDone; }
};

}