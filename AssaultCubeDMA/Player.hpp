#pragma once

#include <Memory.h>
#include "Vectors.hpp"
#include <iostream>
#include <vector>

struct PlayerStruct
{
	uint32_t		VFTable;								// 0x000	0

	Vec3   			PositionHead;							// 0x004	4 -> 16
	char   			Pad0[0x028 - 0x004 - sizeof(Vec3)];		// 220
	Vec3			PositionFeet;							// 0x028	40 -> 52
	float			Yaw;									// 0x034	52 -> 56
	float 			Pitch;									// 0x038	56 -> 60
	char			Pad1[0xE4 - 0x038 - sizeof(float)];		// 168
	int				LastVisibleFrame;						// 0x0E4	228 -> 232
	char			Pad2[0xEC - 0x0E4 - sizeof(int)];		// 4
	int				Health;									// 0x0EC	236 -> 240
	int				Armor;									// 0x0F0	240 -> 244
	char   			Pad3[0x1DC - 0x0F0 - sizeof(int)];		// 232
	int 			Frags;									// 0x1DC	476 -> 480
	char 			Pad4[0x1E4 - 0x1DC - sizeof(int)];		// 4
	int 			Deaths;									// 0x1E4	484 -> 488
	char   			Pad5[0x205 - 0x1E4 - sizeof(int)];		// 29
	char   			Name[16];								// 0x205	517 -> 533
	char			Pad6[0x30C - 0x205 - sizeof(char[16])];	// 247
	int				Team; 									// 0x30C	780 -> 784
};

class Player
{
	private:
		std::unique_ptr<PlayerStruct>	ClassStruct;
		PlayerStruct					*PlayerData		= nullptr;
		uintptr_t						Address 		= 0;
	public:
		Player() = default;
		Player(uintptr_t PlayerPtr);
		Player(uintptr_t Offset, bool IsOffset);

		Player(Player &&Other) noexcept;
		Player &operator=(Player &&Other) noexcept;

		Player(const Player&) = delete;
		Player &operator=(const Player&) = delete;

		PlayerStruct*	operator->()				const;

		Vec2 ScreenHead 							= { };
		Vec2 ScreenFeet 							= { };

		bool	IsVisibleFlag 						= false;
		bool	IsEnemyFlag 						= false;
		float	Distance 							= 0.f;
		float	BoxHeight 							= 0.f;
		float	BoxWidth 							= 0.f;

		bool	IsValid()							const;
		bool	IsAlive()							const;
		bool	IsEnemy(const Player &LocalPlayer)	const;
		bool	IsVisible(int CurrentFrame)			const;

		void 	SetHealth(int Value) 				const;
		void 	SetArmor(int Value) 				const;

		int		GetHealth() 						const;
		int		GetArmor()							const;
		int		GetFrags()							const;
		int 	GetDeaths()							const;
		char*	GetName() 							const;
		int		GetTeam()							const;
		float	GetYaw()							const;
		Vec3	GetHeadPos()						const;
		float	GetPitch()							const;
		Vec3	GetFeetPos()						const;
		float	GetDistanceTo(const Player &Other)	const;
};
