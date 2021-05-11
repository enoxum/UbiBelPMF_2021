#pragma once

#include "core/core.h"

struct HealthBlip
{
	void setWhite() 
	{
		color = { 1.0f, 1.0f, 1.0f };
	}
	void setRed()
	{
		color = { 1.0f, 0.0f, 0.0f};
	}

	bool			side{ false };
	int				idx{ 0 };
	// I know...
	Vector3			color {1.0f, 1.0f, 1.0f};
};