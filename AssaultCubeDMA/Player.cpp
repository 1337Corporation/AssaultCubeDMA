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
