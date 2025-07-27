#include "Player.hpp"

/// <summary>
/// Constructs a Player object by reading player data from a specified memory address.
/// </summary>
/// <param name="PlayerPtr">The memory address (pointer) from which to read the player data.</param>
Player::Player(uintptr_t PlayerPtr)
{
	if (!PlayerPtr == 0)
		return;
	auto Temp = std::make_unique<PlayerStruct>();
	if (!TargetProcess.Read(PlayerPtr, Temp.get(), sizeof(PlayerStruct)))
	{
		std::cout << "Failed to read on PlayerPtr" << std::endl;
		return;
	}
	PlayerData = Temp.get();
	ClassStruct = std::move(Temp);
	Address = PlayerPtr;
}

/// <summary>
/// Constructs a Player object by reading player data from a target process at a specified memory offset.
/// </summary>
/// <param name="Offset">The memory offset from which to read the player pointer.</param>
/// <param name="IsOffset">Indicates whether the provided Offset should be used (true) or ignored (false).</param>
Player::Player(uintptr_t Offset, bool IsOffset)
{
	if (!IsOffset || Offset == 0)
		return;
	uint32_t Ptr32 = TargetProcess.Read<uint32_t>(Offset);
	if (!Ptr32)
	{
		std::cout << "Failed to read 32 bits pointer !" << std::endl;
		return;
	}
	uintptr_t PlayerPtr = static_cast<uintptr_t>(Ptr32);
	if (PlayerPtr == 0)
	{
		std::cout << "Failed to cast 32 -> 64 ptr" << std::endl;
		return;
	}
	auto Temp = std::make_unique<PlayerStruct>();
	if (!TargetProcess.Read(PlayerPtr, Temp.get(), sizeof(PlayerStruct)))
	{
		std::cout << "No player ptr";
		return;
	}
	PlayerData = Temp.get();
	ClassStruct = std::move(Temp);
	Address = PlayerPtr;
}

/// <summary>
/// Move constructor for the Player class. Transfers the resources and state from another Player object to this one.
/// </summary>
/// <param name="other">The Player object to move from. Its resources and state will be transferred to the new object.</param>
Player::Player(Player &&other) noexcept
{
	ClassStruct 	= std::move(other.ClassStruct);
	PlayerData 		= ClassStruct.get();
	Address 		= other.Address;
	ScreenHead 		= other.ScreenHead;
	ScreenFeet 		= other.ScreenFeet;
	IsEnemyFlag 	= other.IsEnemyFlag;
	IsVisibleFlag 	= other.IsVisibleFlag;
	Distance 		= other.Distance;
	BoxHeight 		= other.BoxHeight;
	BoxWidth 		= other.BoxWidth;
	AimDistance	 	= other.AimDistance;
}

/// <summary>
/// Move assignment operator for the Player class. Transfers the resources and data from another Player object to this one, leaving the source in a valid but unspecified state.
/// </summary>
/// <param name="other">The Player object to move from.</param>
/// <returns>A reference to this Player object after the move assignment.</returns>
Player &Player::operator=(Player &&other) noexcept
{
	if (this != &other)
	{
		ClassStruct 	= std::move(other.ClassStruct);
		PlayerData 		= ClassStruct.get();
		Address 		= other.Address;
		ScreenHead 		= other.ScreenHead;
		ScreenFeet 		= other.ScreenFeet;
		IsEnemyFlag 	= other.IsEnemyFlag;
		IsVisibleFlag 	= other.IsVisibleFlag;
		Distance 		= other.Distance;
		BoxHeight 		= other.BoxHeight;
		BoxWidth 		= other.BoxWidth;
		AimDistance	 	= other.AimDistance;
	}
	return *this;
}

/// <summary>
/// Provides access to the underlying PlayerStruct pointer for the Player object.
/// </summary>
/// <returns>A pointer to the PlayerStruct associated with this Player object.</returns>
PlayerStruct *Player::operator->() const
{
	return PlayerData;
}

/// <summary>
/// Checks whether the player is valid by verifying that PlayerData is not null.
/// </summary>
/// <returns>true if PlayerData is not null; otherwise, false.</returns>
bool Player::IsValid() const
{
	if (PlayerData != nullptr)
		return true;
	return false;
}

/// <summary>
/// Checks whether the player is alive.
/// </summary>
/// <returns>true if the player is valid and has health greater than zero; otherwise, false.</returns>
bool Player::IsAlive() const
{
	if (IsValid() && PlayerData->Health > 0)
		return true;
	return false;
}

/// <summary>
/// Determines whether the player is an enemy of the specified local player.
/// </summary>
/// <param name="LocalPlayer">A reference to the local player to compare teams with.</param>
/// <returns>true if the player is valid and on a different team (based on the least significant bit of the team value) than the local player; otherwise, false.</returns>
bool Player::IsEnemy(const Player &LocalPlayer) const
{
	if (IsValid() && (PlayerData->Team & 1) != (LocalPlayer->Team & 1))
		return true;
	return false;
}

/// <summary>
/// Determines whether the player is visible in the specified frame.
/// </summary>
/// <param name="CurrentFrame">The current frame number to check visibility against.</param>
/// <returns>true if the player is valid and was last visible in or after the specified frame; otherwise, false.</returns>
bool Player::IsVisible(int CurrentFrame) const
{
	if (IsValid() && PlayerData->LastVisibleFrame >= CurrentFrame)
		return true;
	return false;
}
