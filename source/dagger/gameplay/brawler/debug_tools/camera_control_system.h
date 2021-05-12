#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"
#include "core/graphics/window.h"
#include "core/input/inputs.h"

using namespace dagger;

namespace brawler {

class CameraControlSystem : public System
{
public:

	String SystemName() override {
		return "Camera Control System";
	}

	void SpinUp() override;
	void Run() override;
	void WindDown() override;

	void OnKeyboardEvent(KeyboardEvent kEvent_);
	void SetControlKeys(EDaggerKeyboard up, EDaggerKeyboard down, EDaggerKeyboard left, EDaggerKeyboard right);

private:
	static float cameraSpeed;
	Vector2 direction {0, 0};
	
	static EDaggerKeyboard up_key;
	static EDaggerKeyboard down_key;
	static EDaggerKeyboard left_key;
	static EDaggerKeyboard right_key;
};

}
