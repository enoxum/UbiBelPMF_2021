
#include "gameplay/PandemicShop/pandemic_tools.h"
#include "tools/plotvar.h"
#include "core/engine.h"
#include "gameplay/common/simple_collisions.h"
#include "core/game/transforms.h"
#include "core/input/inputs.h"
#include "core/graphics/window.h"
#include "gameplay/PandemicShop/pandemic_player_input.h"
#include "core/graphics/sprite.h"

#include <imgui/imgui.h>
#include <spdlog/spdlog.h>


using namespace pandemic_shop;

void CollisionDetectionSystem::Tick()
{
	m_FrameCounter++;
}

void CollisionDetectionSystem::RenderGUI()
{
	ImGui::SetNextWindowSize(ImVec2(200, 60), ImGuiCond_FirstUseEver);
	ImGui::Begin("Collisions");
	{
		auto isPaused = dagger::Engine::s_IsPaused;
		ImGui::Text(isPaused ? "SYSTEMS PAUSED" : "SYSTEMS RUNNING");
	}
	ImGui::Separator();

	//auto &reg = Engine::Registry();
	//auto moving_obj = reg.view<Sprite, Transform, SimpleCollision>();
	//auto static_obj = reg.view<Sprite, Transform, SimpleCollision>();

	//moving_obj.each([&](Sprite& sprite_, Transform& transform_, SimpleCollision col_)
	//	{
	//		const auto& other = col_.colidedWith;
	//		const auto& tr_other = static_obj.get<Transform>(other);
	//		
	//		ImGui::Text("Player position: %f %f", transform_.position.x, transform_.position.y);
	//		// ImGui::Text("Collided: %s", col.colided == true ? "true" : "false");
	//		ImGui::Text("Colided with position: %f %f", tr_other.position.x, tr_other.position.y);
	//	});

	ImGui::End();
}

void CollisionDetectionSystem::SpinUp()
{
	Engine::Dispatcher().sink<GUIRender>().connect<&CollisionDetectionSystem::RenderGUI>(this);
	Engine::Dispatcher().sink<NextFrame>().connect<&CollisionDetectionSystem::Tick>(this);
}

void CollisionDetectionSystem::Run()
{
	
}

void CollisionDetectionSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&CollisionDetectionSystem::Tick>(this);
	Engine::Dispatcher().sink<GUIRender>().disconnect<&CollisionDetectionSystem::RenderGUI>(this);
}
