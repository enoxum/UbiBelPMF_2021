#include "camera_control_system.h"

using namespace dagger;
using namespace brawler;

float CameraControlSystem::cameraSpeed = 150.0f;

EDaggerKeyboard CameraControlSystem::up_key = EDaggerKeyboard::KeyKp8;
EDaggerKeyboard CameraControlSystem::down_key = EDaggerKeyboard::KeyKp2;
EDaggerKeyboard CameraControlSystem::right_key = EDaggerKeyboard::KeyKp6;
EDaggerKeyboard CameraControlSystem::left_key = EDaggerKeyboard::KeyKp4;

void CameraControlSystem::SpinUp()
{
	Engine::Dispatcher().sink<KeyboardEvent>().connect<&CameraControlSystem::OnKeyboardEvent>(this);
}

void CameraControlSystem::WindDown()
{
	Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&CameraControlSystem::OnKeyboardEvent>(this);
}

void CameraControlSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
	if (kEvent_.key == up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
	{
		direction.y = 1;
	}
	else if (kEvent_.key == up_key && kEvent_.action == EDaggerInputState::Released && direction.y > 0)
	{
		direction.y = 0;
	}
	if (kEvent_.key == down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
	{
		direction.y = -1;
	}
	else if (kEvent_.key == down_key && kEvent_.action == EDaggerInputState::Released && direction.y < 0)
	{
		direction.y = 0;
	}

	if (kEvent_.key == right_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
	{
		direction.x = 1;
	}
	else if (kEvent_.key == right_key && kEvent_.action == EDaggerInputState::Released && direction.x > 0)
	{
		direction.x = 0;
	}
	if (kEvent_.key == left_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
	{
		direction.x = -1;
	}
	else if (kEvent_.key == left_key && kEvent_.action == EDaggerInputState::Released && direction.x < 0)
	{
		direction.x = 0;
	}
}

void CameraControlSystem::Run() {
	auto* camera = Engine::GetDefaultResource<Camera>();
	camera->position.x += cameraSpeed * direction.x * Engine::DeltaTime();
	camera->position.y += cameraSpeed * direction.y * Engine::DeltaTime();
	camera->Update();
}

void CameraControlSystem::SetControlKeys(EDaggerKeyboard up_, EDaggerKeyboard down_, EDaggerKeyboard left_, EDaggerKeyboard right_)
{
	up_key = up_;
	down_key = down_;
	left_key = left_;
	right_key = right_;
}