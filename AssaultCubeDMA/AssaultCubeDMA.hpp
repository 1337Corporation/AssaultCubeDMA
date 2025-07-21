#pragma once 

constexpr auto PROCESS_NAME = "ac_client.exe";

namespace Offsets
{
	namespace LocalPlayer
	{
		constexpr size_t Health							= 0xEC;
		constexpr size_t Armor							= 0xF0;
		constexpr size_t Name							= 0x205;

		constexpr size_t PlayerCameraX					= 0x34;
		constexpr size_t PlayerCameraY					= 0x38;

		constexpr size_t PlayerPosX						= 0xFF;
		constexpr size_t PlayerPosY						= 0xFF;
		constexpr size_t PlayerPosZ						= 0x30;
	}

	namespace Guns
	{
		constexpr size_t PistolAmmo						= 0x12C;
		constexpr size_t PistolAvailableAmmo			= 0x108;

		constexpr size_t MachineGunAmmo					= 0x140;
		constexpr size_t MachineGunAvailableAmmo		= 0x11C;

		constexpr size_t GrenadeCount					= 0x144;
	}
}