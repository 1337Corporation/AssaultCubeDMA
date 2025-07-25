#include <iostream>

struct PlayerStruct;

struct WeaponStruct
{
	uint32_t				VFTable;	// 0x000
	int 					Id;			// 0x004
	struct PlayerStruct		*Owner;		// 0x008
};
