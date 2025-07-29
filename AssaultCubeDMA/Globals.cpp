#include "Globals.hpp"

HWND					HwndGame				= nullptr;
HWND					HwndOverlay				= nullptr;
WNDCLASSEX				Wc						= { };
int 					ScreenWidth				= 0;
int 					ScreenHeight			= 0;
int 					LastWidth				= 0;
int 					LastHeight				= 0;
LPDIRECT3D9				pD3D					= nullptr;
LPDIRECT3DDEVICE9		pDevice					= nullptr;
D3DPRESENT_PARAMETERS	d3dpp					= { };
MSG						msg						= { };
