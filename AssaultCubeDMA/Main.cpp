#include "Memory.h"
#include <iostream>
#include <thread>
#include <windows.h>
#include "Player.hpp"
#include "Cheat.hpp"
#include "Globals.hpp"
#include "Render.hpp"
#include "Esp.hpp"
#include "Weapon.hpp"

//int main()
//{
//	if (!TargetProcess.Init(PROCESS_NAME))
//	{
//		while (1) {}
//	}
//	HwndGame = FindGameWindow(TargetProcess.GetPidFromName(PROCESS_NAME));
//	if (!HwndGame)
//	{
//		return -1;
//	}
//
//	ImGuiIO& io = InitOverlay();
//
//	while (msg.message != WM_QUIT)
//	{
//		HandleWindowMessages(msg);
//		UpdateRenderDimensions();
//
//		if (!HandleDeviceResetIfNeeded(LastWidth, LastHeight))
//			break;
//		io.DisplaySize = ImVec2((float)ScreenWidth, (float)ScreenHeight);
//		if (IsGameMinimized())
//		{
//			ShowWindow(HwndOverlay, SW_HIDE);
//			continue;
//		}
//		else
//		{
//			ShowWindow(HwndOverlay, SW_SHOW);
//		}
//		Player localPlayer = InitLocalPlayer();
//		auto Players = BuildPlayerList(localPlayer);
//
//		BeginImGuiFrame(io);
//
//		DrawESP(Players);
//
//		EndImGuiFrame();
//	}
//	Cleanup();
//	return 0;
//}

int main()
{
	if (!TargetProcess.Init(PROCESS_NAME))
	{
		while (1) {}
	}

	Player player(LocalPlayerOffset, true);

	std::cout << player.GetName() << std::endl;

	WeaponStruct* knife;

	TargetProcess.Read(player.GetAddress() + 0x340, &knife, sizeof(knife));

	std::cout << "Knife Address: " << std::hex << knife << std::dec << std::endl;
	//std::cout << "Knife: " << knife->Id << std::endl;

	Player KnifePlayer(uint32_t(knife) + 0x008, true);

	std::cout << "Knife Player Address: " << std::hex << KnifePlayer.GetAddress() << std::dec << std::endl;
	std::cout << "Knife Player Name: " << KnifePlayer.GetName() << std::endl;

	int id;


	// Read the id from the knife weapon structure is working
	TargetProcess.Read((uint32_t)knife + 0x004, &id, sizeof(id));

	std::cout << id << std::endl;

	// Now for reach player i have to get the id -> name of the weapon the display it on the hover tooltip !!! 

	// GOAL FOR TMR
}