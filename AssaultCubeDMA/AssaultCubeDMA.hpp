#pragma once 

constexpr auto PROCESS_NAME = "ac_client.exe";

namespace Offsets
{
	constexpr size_t Health			= 0xEC;
	constexpr size_t MachineGun		= 0x140;
	constexpr size_t Pistol			= 0x12C;

	constexpr size_t PlayerCameraX	= 0x34;
}