#include "Memory.h"
#include <iostream>
#include <thread>
#include <windows.h>
#include "Player.hpp"
#include "Cheat.hpp"
#include "Globals.hpp"
#include "Render.hpp"
#include "Esp.hpp"

int main()
{
	if (!TargetProcess.Init(PROCESS_NAME))
	{
		while (1) {}
	}
	HwndGame = FindGameWindow(TargetProcess.GetPidFromName(PROCESS_NAME));
	if (!HwndGame)
	{
		return -1;
	}

	ImGuiIO& io = InitOverlay();

	while (msg.message != WM_QUIT)
	{
		HandleWindowMessages(msg);
		UpdateRenderDimensions();

		if (!HandleDeviceResetIfNeeded(LastWidth, LastHeight))
			break;
		io.DisplaySize = ImVec2((float)ScreenWidth, (float)ScreenHeight);
		if (IsGameMinimized())
		{
			ShowWindow(HwndOverlay, SW_HIDE);
			continue;
		}
		else
		{
			ShowWindow(HwndOverlay, SW_SHOW);
		}
		Player localPlayer = InitLocalPlayer();
		auto Players = BuildPlayerList(localPlayer);

		BeginImGuiFrame(io);

		DrawESP(Players);

		EndImGuiFrame();
	}
	Cleanup();
	return 0;
}
