#include "Cheat.hpp"

/// <summary>
/// Initializes and configures the local Player with default armor and health values.
/// </summary>
/// <returns>A Player object representing the initialized local Player, with armor and health set to 42. If the Player is not valid, returns the uninitialized Player object.</returns>
Player InitLocalPlayer()
{
	Player LocalPlayer(LocalPlayerOffset, true);
	if (!LocalPlayer.IsValid())
	{
		return LocalPlayer;
	}
	return LocalPlayer;
}

/// <summary>
/// Builds and returns a list of valid, alive Players with Screen coordinates and status information, based on data read from the target process.
/// </summary>
/// <param name="localPlayer">A reference to the local Player, used for comparison and status checks.</param>
/// <returns>A std::vector containing Player objects representing valid, alive Players with updated Screen positions and status flags.</returns>
std::vector<Player> BuildPlayerList(const Player& localPlayer)
{
	float ViewMatrix[16];
	if (!TargetProcess.Read(ViewMatrixOffset, ViewMatrix, sizeof(ViewMatrix)))
	{
		return std::vector<Player>();
	}

	std::vector<Player>	Players;
	int 				PlayersCount	= TargetProcess.Read<int>(PlayersCountOffset);
	uintptr_t			PlayerListPtr	= static_cast<uintptr_t>(TargetProcess.Read<uint32_t>(PlayersListOffset));
	int					CurrentFrame	= TargetProcess.Read<int>(CurrentFrameOffset);

	if (PlayerListPtr == 0 || PlayersCount <= 0 || PlayersCount > MAX_PLAYERS)
	{
		return Players;
	}

	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		uintptr_t	EntryAddress	= PlayerListPtr + i * sizeof(uint32_t);
		uintptr_t	PlayerPtr 		= static_cast<uintptr_t>(TargetProcess.Read<uint32_t>(EntryAddress));

		if (!PlayerPtr)
		{
			continue;
		}

		Player Player(PlayerPtr);
		if (!Player.IsValid() || !Player.IsAlive())
		{
			continue;
		}

		if (ScreenWidth <= 0 || ScreenHeight <= 0)
		{
			continue;
		}

		Vec3		HeadPos			= Player.GetHeadPos();
		Vec3 		FeetPos			= Player.GetFeetPos();

		if (!WorldToScreen(HeadPos, Player.ScreenHead, ViewMatrix, ScreenWidth, ScreenHeight))
		{
			continue;
		}
		if (!WorldToScreen(FeetPos, Player.ScreenFeet, ViewMatrix, ScreenWidth, ScreenHeight))
		{
			continue;
		}

		Player.IsEnemyFlag			= Player.IsEnemy(localPlayer);
		Player.IsVisibleFlag		= Player.IsVisible(CurrentFrame);
		Player.Distance 			= Player.GetDistanceTo(localPlayer);

		ComputePlayerBox(Player);

		Players.push_back(std::move(Player));
		if ((int)Players.size() >= PlayersCount - 1)
		{
			break;
		}
	}
	return Players;
}
