#include "Vectors.hpp"

struct PlayerStruct
{
	Vec3  	Position;	// 0x4
	size_t 	Health;    	// 0xEC
	size_t 	Armour;    	// 0xF0
	size_t 	Frags;     	// 0x1DC
	size_t 	Deaths;    	// 0x1E4
	char  	Name[16];   // 0x205
};
