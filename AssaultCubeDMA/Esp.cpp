#include "Esp.hpp"

/// <summary>
/// Projects a 3D world coordinate to 2D screen space using a transformation matrix.
/// </summary>
/// <param name="World">The 3D world position to project.</param>
/// <param name="Screen">Reference to a Vec2 that will receive the resulting 2D screen coordinates.</param>
/// <param name="M">Pointer to a 4x4 column-major transformation matrix (typically view-projection).</param>
/// <param name="Width">The width of the screen or viewport in pixels.</param>
/// <param name="Height">The height of the screen or viewport in pixels.</param>
/// <returns>True if the world position is successfully projected onto the screen; false if the point is behind the camera or cannot be projected.</returns>
bool WorldToScreen(const Vec3& World, Vec2& Screen, float *M, int Width, int Height)
{
	// Matrix is column-major: index as [row + col * 4]
	float ClipX = World.X * M[0] + World.Y * M[4] + World.Z * M[8] + M[12];
	float ClipY = World.X * M[1] + World.Y * M[5] + World.Z * M[9] + M[13];
	float ClipW = World.X * M[3] + World.Y * M[7] + World.Z * M[11] + M[15];

	if (ClipW < 0.001f) return false;

	float ndcX = ClipX / ClipW;
	float ndcY = ClipY / ClipW;

	Screen.X = (Width / 2.0f) + (ndcX * Width / 2.0f);
	Screen.Y = (Height / 2.0f) - (ndcY * Height / 2.0f);

	return true;
}

/// <summary>
/// Calculates and sets the bounding box dimensions for a player based on their screen head and feet positions.
/// </summary>
/// <param name="Player">Reference to the Player object whose box dimensions will be computed and updated.</param>
void ComputePlayerBox(Player &Player)
{
	float HeightDiff = Player.ScreenHead.Y - Player.ScreenFeet.Y;
	Player.BoxHeight = abs(HeightDiff);
	Player.BoxWidth = Player.BoxHeight * 0.6f; // Adjust ratio as needed
}

/// <summary>
/// Calculates the top-left corner coordinates of a player's bounding box on the screen.
/// </summary>
/// <param name="Player">A reference to the Player object containing screen position and box width information.</param>
/// <returns>An ImVec2 representing the top-left coordinates of the player's bounding box.</returns>
ImVec2 GetBoxTopLeft(const Player &Player)
{
	return ImVec2(Player.ScreenHead.X - Player.BoxWidth / 2, Player.ScreenHead.Y);
}

/// <summary>
/// Calculates the bottom-right corner coordinates of a player's bounding box on the screen.
/// </summary>
/// <param name="Player">A reference to the Player object whose bounding box is being calculated.</param>
/// <returns>An ImVec2 representing the screen coordinates of the bottom-right corner of the player's box.</returns>
ImVec2 GetBoxBottomRight(const Player &Player)
{
	return ImVec2(Player.ScreenHead.X + Player.BoxWidth / 2, Player.ScreenFeet.Y);
}

/// <summary>
/// Determines the color to use for a player's box based on their enemy and visibility status.
/// </summary>
/// <param name="Player">A reference to the Player object whose box color is to be determined.</param>
/// <returns>An ImU32 value representing the color: red for visible enemies, yellow for hidden enemies, and green for non-enemies.</returns>
ImU32 GetBoxColor(const Player &Player)
{
	if (Player.IsEnemyFlag)
	{
		return Player.IsVisibleFlag ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 0, 255);
	}
	return IM_COL32(0, 255, 0, 255);
}

/// <summary>
/// Draws a rectangular box around a player using the specified drawing list and style parameters.
/// </summary>
/// <param name="DrawList">Pointer to the ImDrawList used for rendering the box. Must not be null.</param>
/// <param name="Player">Reference to the Player object for which the box will be drawn.</param>
/// <param name="Rounding">The corner rounding radius for the box.</param>
/// <param name="Thickness">The thickness of the box outline.</param>
void DrawPlayerBox(ImDrawList *DrawList, const Player &Player, float Rounding, float Thickness)
{
	if (!DrawList)
	{
		return;
	}

	ImVec2 TopLeft = GetBoxTopLeft(Player);
	ImVec2 BottomRight = GetBoxBottomRight(Player);
	ImU32 Color = GetBoxColor(Player);

	DrawList->AddRect(TopLeft, BottomRight, Color, Rounding, 0, Thickness);
}

/// <summary>
/// Draws ESP (Extra Sensory Perception) overlays for enemy players using ImGui.
/// </summary>
/// <param name="Players">A vector containing Player objects to be processed for ESP drawing.</param>
void DrawESP(const std::vector<Player>& Players)
{
	if (!ImGui::GetCurrentContext())
	{
		return;
	}

	ImDrawList* DrawList = ImGui::GetBackgroundDrawList(); // use BackgroundDrawList to avoid z-order issues
	if (!DrawList)
	{
		return;
	}

	for (const auto &Player : Players)
	{
		DrawPlayerBox(DrawList, Player, 0.0f, 1.0f);
	}
}
