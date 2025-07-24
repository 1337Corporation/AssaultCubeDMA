#include "Player.hpp"

Player::Player(uintptr_t Offset, bool IsOffset)
{
	if (!IsOffset)
	{
		return;
	}
	uint32_t Ptr32 = TargetProcess.Read<uint32_t>(Offset);
	if (!Ptr32)
	{
		std::cout << "Failed to read 32 bits pointer !" << std::endl;
		return;
	}
	uintptr_t PlayerPtr = static_cast<uintptr_t>(Ptr32);
	if (PlayerPtr == 0)
	{
		return;
	}
	auto Temp = std::make_unique<PlayerStruct>();
	if (!TargetProcess.Read(PlayerPtr, Temp.get(), sizeof(PlayerStruct)))
	{
		std::cout << "No player ptr";
		return;
	}
	Ptr = Temp.get();
	ClassStruct = std::move(Temp);
	Address = PlayerPtr;
}
