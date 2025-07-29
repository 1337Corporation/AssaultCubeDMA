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
/// <param name="Player">A reference to the Pweft coordinates of the player's bounding box.</returns>
static ImVec2 GetBoxTopLeft(const Player &Player)
{
	return ImVec2(Player.ScreenHead.X - Player.BoxWidth / 2, Player.ScreenHead.Y);
}

/// <summary>
/// Calculates the bottom-right corner coordinates of a player's bounding box on the screen.
/// </summary>
/// <param name="Player">A reference to the Player object whose bounding box is being calculated.</param>
/// <returns>An ImVec2 representing the screen coordinates of the bottom-right corner of the player's box.</returns>
static ImVec2 GetBoxBottomRight(const Player &Player)
{
	return ImVec2(Player.ScreenHead.X + Player.BoxWidth / 2, Player.ScreenFeet.Y);
}

/// <summary>
/// Determines the color to use for a player's box based on their enemy and visibility status.
/// </summary>
/// <param name="Player">A reference to the Player object whose box color is to be determined.</param>
/// <returns>An ImU32 value representing the color: red for visible enemies, yellow for hidden enemies, and green for non-enemies.</returns>
static ImU32 GetBoxColor(const Player &Player)
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
static void DrawPlayerBox(ImDrawList *DrawList, const Player &Player, float Rounding, float Thickness)
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
/// Draws the player's name horizontally centered above their bounding box using the specified draw list.
/// </summary>
/// <param name="DrawList">Pointer to the ImDrawList used for rendering the text.</param>
/// <param name="Player">Reference to the Player object whose name will be drawn.</param>
static void DrawPlayerDistance(ImDrawList* DrawList, const Player &Player)
{
	char buffer[64];

	snprintf(buffer, sizeof(buffer), "[%.1fm]", Player.Distance);

	ImVec2 TopLeft = GetBoxTopLeft(Player);
	ImVec2 BottomRight = GetBoxBottomRight(Player);
	ImVec2 TextSize = ImGui::CalcTextSize(buffer);

	float BoxCenterX = (TopLeft.x + BottomRight.x) / 2.0f;

	ImVec2 Position = ImVec2(BoxCenterX - TextSize.x / 2.0f, TopLeft.y - TextSize.y - 5.0f);

	DrawList->AddText(Position, IM_COL32(255, 255, 255, 255), buffer);
}

static void DrawPlayerHealth(ImDrawList *DrawList, const Player &Player)
{
    if (!DrawList || !Player.IsEnemyFlag)
    {
        return;
    }

    ImVec2 TopLeft = GetBoxTopLeft(Player);
    ImVec2 BottomRight = GetBoxBottomRight(Player);

    float BarWidth = 4.0f;
    float BarHeight = BottomRight.y - TopLeft.y;
    float BarOffset = 8.0f;

    ImVec2 BarTopLeft = ImVec2(TopLeft.x - BarOffset - BarWidth, TopLeft.y);
    ImVec2 BarBottomRight = ImVec2(TopLeft.x - BarOffset, BottomRight.y);

    DrawList->AddRectFilled(BarTopLeft, BarBottomRight, IM_COL32(0, 0, 0, 150));

    float HealthPercent = Player.GetHealth() / 100.0f;
    if (HealthPercent < 0.0f) HealthPercent = 0.0f;
    if (HealthPercent > 1.0f) HealthPercent = 1.0f;

    float FillHeight = BarHeight * HealthPercent;
    ImVec2 FillTopLeft = ImVec2(BarTopLeft.x, BarBottomRight.y - FillHeight);
    ImVec2 FillBottomRight = BarBottomRight;

    ImU32 HealthColor;
    if (HealthPercent > 0.6f)
        HealthColor = IM_COL32(0, 255, 0, 255); // Green
    else if (HealthPercent > 0.3f)
        HealthColor = IM_COL32(255, 255, 0, 255); // Yellow
    else
        HealthColor = IM_COL32(255, 0, 0, 255); // Red

    DrawList->AddRectFilled(FillTopLeft, FillBottomRight, HealthColor);
    DrawList->AddRect(BarTopLeft, BarBottomRight, IM_COL32(255, 255, 255, 100), 0.0f, 0, 1.0f);
}

/// <summary>
/// Draws a snapline from the center bottom of the screen to the player's feet position.
/// </summary>
/// <param name="DrawList">Pointer to the ImDrawList used for rendering the snapline.</param>
/// <param name="Player">Reference to the Player object whose snapline will be drawn.</param>
static void DrawPlayerSnapline(ImDrawList *DrawList, const Player &Player)
{
	if (Player.IsEnemyFlag)
	{
		ImVec2 ScreenBottomCenter = ImVec2(ScreenWidth / 2, ScreenHeight);
		ImVec2 PlayerScreenFeet = ConvertVec2ToImVec2(Player.ScreenFeet);
		DrawList->AddLine(ScreenBottomCenter, PlayerScreenFeet, IM_COL32(255, 255, 255, 255), 1.0f);
	}
}

/// <summary>
/// Checks if the crosshair (center of screen) is hovering over a player's bounding box.
/// </summary>
/// <param name="Player">Reference to the Player object to check.</param>
/// <returns>True if the crosshair is hovering over the player's box, false otherwise.</returns>
static bool IsCrosshairHoveringPlayer(const Player &Player)
{
	float CrosshairX = ScreenWidth * 0.5f;
	float CrosshairY = ScreenHeight * 0.5f;
	constexpr float Padding = 10.0f;

	const float BoxLeft = Player.ScreenHead.X - Player.BoxWidth * 0.5f - Padding;
	const float BoxRight = Player.ScreenHead.X + Player.BoxWidth * 0.5f + Padding;
	const float BoxTop = Player.ScreenHead.Y - Padding;
	const float BoxBottom = Player.ScreenFeet.Y + Padding;

	return (CrosshairX >= BoxLeft && CrosshairX <= BoxRight &&
			CrosshairY >= BoxTop && CrosshairY <= BoxBottom);
}

/// <summary>
/// Draws a popup tooltip with detailed player information when crosshair is hovering over a player.
/// </summary>
/// <param name="Player">Reference to the Player object whose information will be displayed.</param>
static void DrawPlayerHoverPopup(const Player &Player)
{
	// Fixed position in top-left area of screen
	constexpr float PopupX = 50.0f;
	constexpr float PopupY = 50.0f;

	ImGui::SetNextWindowPos(ImVec2(PopupX, PopupY), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.9f);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 0.0f)); // Fixed width, auto height

	ImGui::Begin("##PlayerInfo", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav);

	// Player name and basic info
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Player: %s", Player.GetName());
	ImGui::Separator();

	// Health and armor with color coding
	const float healthPercent = Player.GetHealth() / 100.0f;
	const ImVec4 healthColor = (healthPercent > 0.6f) ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) :
							   (healthPercent > 0.3f) ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) :
							   ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	ImGui::TextColored(healthColor, "Health: %d/100", Player.GetHealth());
	ImGui::Text("Armor: %d", Player.GetArmor());
	ImGui::Text("Distance: %.1f m", Player.Distance);
	ImGui::Text("Team: %d", Player.GetTeam());

	// Visibility status
	ImGui::TextColored(Player.IsVisibleFlag ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
		"Status: %s", Player.IsVisibleFlag ? "Visible" : "Hidden");

	ImGui::End();
}

/// <summary>
/// Draws ESP (Extra Sensory Perception) overlays for enemy players using ImGui.
/// </summary>
/// <param name="Players">A vector containing Player objects to be processed for ESP drawing.</param>
void DrawESP(const std::vector<Player> &Players)
{
	if (!ImGui::GetCurrentContext())
		return;

	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
	if (!DrawList)
		return;

	const Player* HoveredPlayer = nullptr;

	for (const auto &Player : Players)
	{
		DrawPlayerBox(DrawList, Player, 0.0f, 1.0f);
		DrawPlayerDistance(DrawList, Player);
		DrawPlayerSnapline(DrawList, Player);
		DrawPlayerHealth(DrawList, Player);

		// Check hover only for enemies through walls (not visible)
		if (Player.IsEnemyFlag && IsCrosshairHoveringPlayer(Player))
		{
			HoveredPlayer = &Player;
		}
	}

	// Draw popup for hovered player (rendered last to ensure it's on top)
	if (HoveredPlayer)
	{
		DrawPlayerHoverPopup(*HoveredPlayer);
	}
}
