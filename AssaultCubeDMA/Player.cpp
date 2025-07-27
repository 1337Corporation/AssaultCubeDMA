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

/// <summary>
/// Retrieves the player's current health value.
/// </summary>
/// <returns>The player's health if the player is valid; otherwise, returns 0.</returns>
int Player::GetHealth() const
{
	if (IsValid())
		return PlayerData->Health;
	return 0;
}

/// <summary>
/// Retrieves the player's armor value.
/// </summary>
/// <returns>The player's armor value if the player is valid; otherwise, returns 0.</returns>
int Player::GetArmor() const
{
	if (IsValid())
		return PlayerData->Armor;
	return 0;
}

/// <summary>
/// Retrieves the name of the player if the player is valid.
/// </summary>
/// <returns>A pointer to the player's name if the player is valid; otherwise, nullptr.</returns>
char *Player::GetName() const
{
	if (IsValid())
		return PlayerData->Name;
	return nullptr;
}

/// <summary>
/// Returns the player's team identifier, or -1 if the player is invalid.
/// </summary>
/// <returns>The team identifier (the least significant bit of PlayerData->Team) if the player is valid; otherwise, -1.</returns>
int Player::GetTeam() const
{
	if (IsValid())
		return PlayerData->Team & 1; // Assuming Team is a bitmask, return the first bit
	return -1;
}

/// <summary>
/// Retrieves the yaw angle of the player.
/// </summary>
/// <returns>The player's yaw angle if the player is valid; otherwise, returns 0.0f.</returns>
float Player::GetYaw() const
{
	if (IsValid())
		return PlayerData->Yaw;
	return 0.f;
}

/// <summary>
/// Retrieves the pitch value of the player.
/// </summary>
/// <returns>The player's pitch value if the player is valid; otherwise, returns 0.0f.</returns>
float Player::GetPitch() const
{
	if (IsValid())
		return PlayerData->Pitch;
	return 0.f;
}

/// <summary>
/// Returns the position of the player's head in 3D space.
/// </summary>
/// <returns>A Vec3 representing the player's head position if the player is valid; otherwise, returns a zero vector (0.f, 0.f, 0.f).</returns>
Vec3 Player::GetHeadPos() const
{
	if (IsValid())
		return (PlayerData->PositionHead + Vec3(0.f, 0.f, 0.75f));
	return Vec3{0.f, 0.f, 0.f};
}

/// <summary>
/// Returns the position of the player's feet in 3D space.
/// </summary>
/// <returns>A Vec3 representing the player's feet position if the player is valid; otherwise, a zero vector (0.f, 0.f, 0.f).</returns>
Vec3 Player::GetFeetPos() const
{
	if (IsValid())
		return PlayerData->PositionFeet;
	return Vec3{0.f, 0.f, 0.f};
}

/// <summary>
/// Calculates the distance from this player to another player.
/// </summary>
/// <param name="Other">A reference to the other Player object to which the distance is measured.</param>
/// <returns>The distance between the head positions of the two players if both are valid; otherwise, returns 0.0f.</returns>
float Player::GetDistanceTo(const Player &Other) const
{
	if (IsValid() && Other.IsValid())
		return GetHeadPos().Distance(Other.GetHeadPos());
	return 0.f;
}

/// <summary>
/// Sets the player's health to the specified value.
/// </summary>
/// <param name="Value">The new health value to set for the player.</param>
void Player::SetHealth(int Value) const
{
	if (Address)
	{
		TargetProcess.Write<int>(Address + 0xEC, Value);
	}
}

/// <summary>
/// Sets the player's armor value in memory.
/// </summary>
/// <param name="Value">The new armor value to set for the player.</param>
void Player::SetArmor(int Value) const
{
	if (Address)
	{
		TargetProcess.Write<int>(Address + 0xF0, Value);
	}
}
