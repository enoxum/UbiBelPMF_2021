#pragma once
#include "source/dagger/core/system.h"
#include "source/dagger/core/core.h"
#include "source/dagger/core/engine.h"

using namespace dagger;

struct pauseESC : public System {
    public:
     pauseESC();
     virtual String SystemName();
     void SpinUp();
     void WindDown();
     void OnKeyboardEvent(KeyboardEvent kEvent_);
};