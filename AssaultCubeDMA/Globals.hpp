#pragma once

#include <cstdint>
#include <d3d9.h>

constexpr auto  				PROCESS_NAME	= "ac_client.exe";
constexpr int					MAX_PLAYERS		= 32;

constexpr uintptr_t 			LocalPlayerOffset 		= 0x58AC00;
constexpr uintptr_t 			PlayersCountOffset		= 0x58AC0C;
constexpr uintptr_t 			PlayersListOffset		= 0x58AC04;
constexpr uintptr_t 			ViewMatrixOffset		= 0x57DFD0;
constexpr uintptr_t 			CurrentFrameOffset		= 0x57F10C;
constexpr uintptr_t 			VisibilityCheckOffset	= 0x58A918;

extern HWND						HwndGame;
extern HWND						HwndOverlay;
extern WNDCLASSEX				Wc;
extern int 						ScreenWidth;
extern int 						ScreenHeight;
extern int                      LastWidth;
extern int                      LastHeight;
extern LPDIRECT3D9				pD3D;
extern LPDIRECT3DDEVICE9		pDevice;
extern D3DPRESENT_PARAMETERS    d3dpp;
extern MSG						msg;
