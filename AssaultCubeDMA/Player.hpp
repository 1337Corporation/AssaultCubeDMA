#include <Memory.h>
#include "Vectors.hpp"
#include <iostream>

struct PlayerStruct
{
	uint32_t	VFTable;		// 0x000

	Vec3   		Position;		// 0x004
	char   		Pad1[0x0EC - 0x004 - sizeof(Vec3)];
	int			Health;			// 0x0EC
	int			Armor;			// 0x0F0
	char   		Pad2[0x1DC - 0x0F0 - sizeof(int)];
	int 		Frags;			// 0x1DC
	char 		Pad3[0x1E4 - 0x1DC - sizeof(int)];
	int 		Deaths;			// 0x1E4
	char   		Pad4[0x205 - 0x1E4 - sizeof(int)];
	char   		Name[16];		// 0x205
	char		Pad5[0x30C - 0x205 - sizeof(char[16])];
	int			Team; 			// 0x30C
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

		bool IsValid() const				{ return Ptr != nullptr; };

		void SetHealth(int Value) 	const		{ if (Address) { TargetProcess.Write<int>(Address + 0xEC, Value); } }
		void SetArmor(int Value) 	const		{ if (Address) { TargetProcess.Write<int>(Address + 0xF0, Value); } }

		int GetHealth() 			const		{ return Ptr->Health; 	};
		int GetArmor()				const		{ return Ptr->Armor;	};
		char *GetName() 			const		{ return Ptr->Name; 	};
};
