#pragma once

#include "core/core.h"
#include "core/system.h"

#include "tools/toolmenu.h"

using namespace dagger;

namespace brawler
{

class DebugGui
    : public System
    , public Publisher<ToolMenuRender>
{
    public:
        inline String SystemName() { return "Brawler Debug System"; }

        void SpinUp() override;
        void Run() override;
        void WindDown() override;

    protected:
        void RenderToolMenu();
        void RenderDebugWindow();
        void OnEndOfFrame();

    private:
        bool debugActive { true };

        bool resetPosition { false };
};

}