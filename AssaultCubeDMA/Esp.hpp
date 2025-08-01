#pragma once

#include "Globals.hpp"
#include "imgui.h"
#include "Vectors.hpp"
#include "Player.hpp"
#include <vector>

bool	WorldToScreen(const Vec3 &World, Vec2 &Screen, float *M, int Width, int Height);
void	DrawESP(const std::vector<Player> &Players);
void	ComputePlayerBox(Player &Player);

namespace Colors
{
	namespace U32
	{
		constexpr ImU32		White				= IM_COL32(255, 255, 255, 255);
		constexpr ImU32 	Green			 	= IM_COL32(0, 255, 0, 255);
		constexpr ImU32		Yellow				= IM_COL32(255, 255, 0, 255);
		constexpr ImU32 	Red					= IM_COL32(255, 0, 0, 255);
		constexpr ImU32 	Purple				= IM_COL32(128, 0, 128, 255);
	}

	namespace Vec4
	{
		constexpr ImVec4	Yellow				= ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		constexpr ImVec4 	Green				= ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		constexpr ImVec4	Red					= ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		constexpr ImVec4	Purple				= ImVec4(0.5f, 0.0f, 0.5f, 1.0f);
	}
}

namespace Sizes
{
	// Player Boxes
	constexpr float		PlayerBoxRounding	= 0.0f;
	constexpr float 	PlayerBoxThickness	= 1.0f;

	// SnapLine
	constexpr float 	SnapLineThickness	= 1.0f;

	// Health Bar
	constexpr float 	HealthBarWidth		= 3.0f;
	constexpr float 	HealthBarOffset		= 6.0f;

	// Player Hover Popup
	constexpr ImVec2	HoverWindowSize		= ImVec2(200.0f, 0.0f);
}

namespace Positions
{
	// Player Hover Popup (from top left of screen)
	constexpr float 	PopupX 				= 150.0f;
	constexpr float 	PopupY 				= 150.0f;
}
