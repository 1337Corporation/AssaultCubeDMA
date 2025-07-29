#pragma once

#include "Globals.hpp"
#include "imgui.h"
#include "Vectors.hpp"
#include "Player.hpp"
#include <vector>

bool 	WorldToScreen(const Vec3 &World, Vec2 &Screen, float *M, int Width, int Height);
void 	DrawESP(const std::vector<Player> &Players);

void 	ComputePlayerBox(Player& Player);
ImVec2 	GetBoxTopLeft(const Player& Player);
ImVec2 	GetBoxBottomRight(const Player& Player);
ImU32 	GetBoxColor(const Player& Player);

void 	DrawPlayerBox(ImDrawList* DrawList, const Player& Player, float Rounding, float Thickness = 1.0f);
