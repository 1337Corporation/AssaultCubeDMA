#pragma once

#include "Globals.hpp"
#include "Player.hpp"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <Windows.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

LRESULT WINAPI	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool			CreateOverlayWindow();
bool			CreateDevice();
ImGuiIO         &InitOverlay();
void			BeginImGuiFrame(ImGuiIO& io);
void			EndImGuiFrame();
void			Cleanup();
HWND			FindGameWindow(DWORD PID);
void			UpdateRenderDimensions();
bool			IsGameMinimized();
void			HandleWindowMessages(MSG& msg);
bool			HandleDeviceResetIfNeeded(int& LastWidth, int& LastHeight);
