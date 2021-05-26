#include "gameplay/roboship/roboship_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"

#include "tools/diagnostics.h"

struct RExpandBackground
{
	int count;
	Float32 x;
};

struct RBackdrop {

	static void RoboshipCreateBackdrop(Float32 background_pos_x, Float32 terrain_pos_x);
	void expand_background(RExpandBackground&);
};