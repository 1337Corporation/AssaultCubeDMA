#pragma once

#include "Globals.hpp"
#include "imgui.h"
#include "Vectors.hpp"
#include "Player.hpp"
#include <vector>

bool	WorldToScreen(const Vec3 &World, Vec2 &Screen, float *M, int Width, int Height);
void	DrawESP(const std::vector<Player> &Players);
void	ComputePlayerBox(Player &Player);

namespace Text
{
	constexpr ImU32 WhiteTextColor		= IM_COL32(255, 255, 255, 255);
}

namespace Colors
{
	// Player Boxes
	constexpr ImU32 BoxVisibleEnnemy	= IM_COL32(255, 0, 0, 255);
	constexpr ImU32 BoxHiddenEnnemy		= IM_COL32(255, 255, 0, 255);
	constexpr ImU32 BoxFriendly			= IM_COL32(0, 255, 0, 255);


	constexpr ImU32 SnapLineColor 		= IM_COL32(255, 255, 255, 255);
	constexpr ImU32 HealthColorGood 	= IM_COL32(0, 255, 0, 255);
	constexpr ImU32 HealthColorMedium	= IM_COL32(255, 255, 0, 255);
	constexpr ImU32 HealthColorBad		= IM_COL32(255, 0, 0, 255);
}

namespace Sizes
{
	// Player Boxes
	constexpr float	PlayerBoxRounding	= 0.0f;
	constexpr float PlayerBoxThickness	= 1.0f;

	// SnapLine
	constexpr float SnapLineThickness	= 1.0f;
}
