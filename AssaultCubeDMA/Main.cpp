#include <iostream>
#include <thread>
#include <windows.h>

#include "Memory.h"
#include "Player.hpp"
#include "Cheat.hpp"
#include "Globals.hpp"
#include "Render.hpp"
#include "Esp.hpp"

static void RenderCheatPresence()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize(Sizes::HoverWindowSize); // Fixed width, auto height

	ImGui::Begin
	(
		"##CheatPresence",
		nullptr,
		ImGuiWindowFlags_NoTitleBar 		|
		ImGuiWindowFlags_NoResize 			|
		ImGuiWindowFlags_NoMove				|
		ImGuiWindowFlags_NoSavedSettings 	|
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav
	);

	ImGui::TextColored(Colors::Vec4::Green, "FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::TextColored(Colors::Vec4::Purple, "made by kaveOO");
	ImGui::TextColored(Colors::Vec4::Red, "Press INSERT to exit !");

	ImGui::End();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!TargetProcess.Init(PROCESS_NAME))
	{
		std::cerr << "[ERROR] Failed to initialize process: " << PROCESS_NAME << std::endl;
		return -1;
	}

	HwndGame = FindGameWindow(TargetProcess.GetPidFromName(PROCESS_NAME));
	if (!HwndGame)
	{
		std::cerr << "[ERROR] Failed to find game window: " << PROCESS_NAME << std::endl;
		return -1;
	}

	ImGuiIO &Io = InitOverlay();

	while (msg.message != WM_QUIT)
	{
		HandleWindowMessages(msg);
		UpdateRenderDimensions();

		if (!HandleDeviceResetIfNeeded(LastWidth, LastHeight))
		{
			break;
		}

		if (GetAsyncKeyState(VK_INSERT))
		{
			break;
		}

		Io.DisplaySize 		= ImVec2((float)ScreenWidth, (float)ScreenHeight);
		Player	LocalPlayer = InitLocalPlayer();
		auto	Players 	= BuildPlayerList(LocalPlayer);

		BeginImGuiFrame(Io);

		DrawESP(Players);
		RenderCheatPresence();

		EndImGuiFrame();
	}
	Cleanup();
	return 0;
}
