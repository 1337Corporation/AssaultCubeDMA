#pragma once

#include "Globals.hpp"
#include "imgui.h"
#include "Vectors.hpp"
#include "Player.hpp"
#include <vector>

bool 	WorldToScreen(const Vec3 &World, Vec2 &Screen, float *M, int Width, int Height);
void 	DrawESP(const std::vector<Player> &Players);
void	ComputePlayerBox(Player &Player);