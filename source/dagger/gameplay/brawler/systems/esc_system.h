#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"
#include "core/input/inputs.h"

using namespace dagger;

namespace brawler
{

class EscSystem : public System
{
public:
    EscSystem();

    inline String SystemName() { return "ESC System"; }

    void SpinUp() override;
    void WindDown() override;
    void Run() override;

private:

    void OnKeyboardEvent(KeyboardEvent kEvent_);
};

}