#include <Memory.h>
#include "Vectors.hpp"
#include <iostream>

struct WeaponStruct;

struct PlayerStruct
{
	uint32_t		VFTable;								// 0x000

	Vec3   			Position;								// 0x004
	char   			Pad1[0x0EC - 0x004 - sizeof(Vec3)];
	int				Health;									// 0x0EC
	int				Armor;									// 0x0F0
	char   			Pad2[0x1DC - 0x0F0 - sizeof(int)];
	int 			Frags;									// 0x1DC
	char 			Pad3[0x1E4 - 0x1DC - sizeof(int)];
	int 			Deaths;									// 0x1E4
	char   			Pad4[0x205 - 0x1E4 - sizeof(int)];
	char   			Name[16];								// 0x205
	char			Pad5[0x30C - 0x205 - sizeof(char[16])];
	int				Team; 									// 0x30C

	char			Pad6[0x33C - 0x30C - sizeof(int)];

	WeaponStruct	*Knife;									// 0x33C
	WeaponStruct	*Pistol;								// 0x340
	WeaponStruct	*Carabine;								// 0x344
	WeaponStruct	*Shotgun;								// 0x348
	WeaponStruct	*Subgun;								// 0x34C
	WeaponStruct	*Sniper;								// 0x350
	WeaponStruct	*Assault;								// 0x354
	WeaponStruct	*Grenade;								// 0x358
	WeaponStruct	*Akimbo;								// 0x35C
	WeaponStruct	*LastWeapon;							// 0x360
	WeaponStruct	*CurrentWeapon;							// 0x364
	WeaponStruct	*Unknown;								// 0x368 This is displaying current weapon but we already having it
	WeaponStruct	*PrimaryWeapon;							// 0x36C
	WeaponStruct	*Unknown1;								// 0x370 This is always displaying Assault gun even when playing sniper
};

class Player
{
	private:
		std::unique_ptr<PlayerStruct>	ClassStruct;
		PlayerStruct					*Ptr			= nullptr;
		uintptr_t						Address 		= 0;
	public:
		Player(uintptr_t Offset, bool IsOffset);
		~Player() { std::cout << "Player has been destroyed" << std::endl; };

		bool IsValid()				const		{ return Ptr != nullptr; };
		bool IsAlive()				const		{ return Ptr && Ptr->Health > 0; };

		void SetHealth(int Value) 	const		{ if (Address) { TargetProcess.Write<int>(Address + 0xEC, Value); } }
		void SetArmor(int Value) 	const		{ if (Address) { TargetProcess.Write<int>(Address + 0xF0, Value); } }

		int GetHealth() 			const		{ return Ptr->Health; 	};
		int GetArmor()				const		{ return Ptr->Armor;	};
		char *GetName() 			const		{ return Ptr->Name; 	};
};
