#pragma once 
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include <deque>
#include <map>

using namespace dagger;

enum struct Action {PICK, IDLE, UP, DOWN, RIGHT, LEFT};

struct AICommand
{
    Vector2 current, previous, next;
    Action curr_action = Action::IDLE;
    bool finished = true;
  
};
// struct KarenAI{
//     AICommand current;
//     std::deque<AICommand>previous;
// };

class AISystem : public System
{
private: 
    int border_height = 20;
    int border_width = 20;
    float delay;
    float counter;
    int depth;
public:

    AISystem() { delay = 1.0f; counter = 0.0f; depth = 3;};
    inline String SystemName() { return "AI System"; }

    void Run() override;

};
