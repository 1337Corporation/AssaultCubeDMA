#include "Vectors.hpp"

struct PlayerStruct
{
	Vec3 	Position;	// 0x4 -> 0xC
	size_t 	Health;		// 0xEC
	size_t 	Armour;		// 0xF0
	char	Name[16];	// 0x205
};
