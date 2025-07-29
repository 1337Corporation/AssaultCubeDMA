#include "Render.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Creates and displays a transparent overlay window on top of all Other windows.
/// </summary>
/// <returns>Returns true if the overlay window was created and shown successfully.</returns>
bool CreateOverlayWindow()
{
	Wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		L"OverlayClass",
		nullptr
	};

	RegisterClassEx(&Wc);
	HWND parentWindow = CreateWindowEx(0, L"STATIC", nullptr, WS_DISABLED,
		0, 0, 0, 0, nullptr, nullptr, Wc.hInstance, nullptr);

	HwndOverlay = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_LAYERED,
		L"OverlayClass", L"Overlay",
		WS_POPUP, 100, 100, 800, 600,
		parentWindow, nullptr, Wc.hInstance, nullptr
	);

	SetLayeredWindowAttributes(HwndOverlay, RGB(0, 0, 0), 255, LWA_COLORKEY);
	ShowWindow(HwndOverlay, SW_SHOW);
	return true;
}

/// <summary>
/// Initializes a Direct3D device with specified parameters.
/// </summary>
/// <returns>Returns true if the Direct3D device was successfully created; otherwise, returns false.</returns>
bool CreateDevice()
{
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D) return false;

	d3dpp = {};
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = HwndOverlay;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;

	return SUCCEEDED(pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, HwndOverlay,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice));
}

/// <summary>
/// Initializes the overlay window and sets up the ImGui context for rendering with Win32 and DirectX 9.
/// </summary>
/// <returns>A reference to the ImGuiIO object associated with the created ImGui context.</returns>
ImGuiIO& InitOverlay()
{
	CreateOverlayWindow();
	CreateDevice();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(HwndOverlay);
	ImGui_ImplDX9_Init(pDevice);

	return io;
}

/// <summary>
/// Initializes a new ImGui frame and updates mouse input state.
/// </summary>
/// <param name="io">Reference to the ImGuiIO object used for input and configuration.</param>
void BeginImGuiFrame(ImGuiIO &io)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	ImGui::NewFrame();
}

/// <summary>
/// Finalizes and renders the current ImGui frame using DirectX 9.
/// </summary>
void EndImGuiFrame()
{
	ImGui::Render();
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (pDevice->BeginScene() >= 0)
	{
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		pDevice->EndScene();
	}
	pDevice->Present(NULL, NULL, NULL, NULL);
}

/// <summary>
/// Releases resources and performs cleanup for ImGui and Direct3D 9, and unregisters the application window class.
/// </summary>
void Cleanup()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (pDevice) pDevice->Release();
	if (pD3D) pD3D->Release();

	UnregisterClass(Wc.lpszClassName, Wc.hInstance);
}

/// <summary>
/// Finds the main visible window associated with a given process ID.
/// </summary>
/// <param name="pid">The process ID of the game whose window is to be found.</param>
/// <returns>The handle to the main visible window (HWND) for the specified process, or nullptr if no such window is found.</returns>
HWND FindGameWindow(DWORD pid)
{
	HWND hwnd = nullptr;
	while ((hwnd = FindWindowEx(nullptr, hwnd, nullptr, nullptr)))
	{
		DWORD windowPID = 0;
		GetWindowThreadProcessId(hwnd, &windowPID);
		if (windowPID == pid && IsWindowVisible(hwnd))
		{
			return hwnd;
		}
	}
	return nullptr;
}

/// <summary>
/// Updates the dimensions and position of the overlay window to match the client area of the game window.
/// </summary>
void UpdateRenderDimensions()
{
	RECT clientRect;
	GetClientRect(HwndGame, &clientRect);
	POINT topLeft = { 0, 0 };
	ClientToScreen(HwndGame, &topLeft);

	ScreenWidth = clientRect.right;
	ScreenHeight = clientRect.bottom;
	MoveWindow(HwndOverlay, topLeft.x, topLeft.y, ScreenWidth, ScreenHeight, TRUE);
}

/// <summary>
/// Determines whether the game window is currently minimized.
/// </summary>
/// <returns>True if the game window is minimized; otherwise, false.</returns>
bool IsGameMinimized()
{
	WINDOWPLACEMENT placement = { sizeof(WINDOWPLACEMENT) };
	GetWindowPlacement(HwndGame, &placement);
	return placement.showCmd == SW_SHOWMINIMIZED;
}

/// <summary>
/// Processes pending window messages by retrieving, translating, and dispatching them.
/// </summary>
/// <param name="msg">A reference to an MSG structure that receives message information.</param>
void HandleWindowMessages(MSG& msg)
{
	if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/// <summary>
/// Handles device reset if the Screen size has changed, updating device objects as needed.
/// </summary>
/// <param name="LastWidth">Reference to the last known Screen Width. Updated if a reset occurs.</param>
/// <param name="LastHeight">Reference to the last known Screen Height. Updated if a reset occurs.</param>
/// <returns>True if the device is ready for use after checking or resetting; false if the device reset failed.</returns>
bool HandleDeviceResetIfNeeded(int &LastWidth, int &LastHeight)
{
	if (ScreenWidth == LastWidth && ScreenHeight == LastHeight) return true;
	if (ImGui::IsAnyMouseDown()) return true;

	LastWidth = ScreenWidth;
	LastHeight = ScreenHeight;

	ImGui_ImplDX9_InvalidateDeviceObjects();

	d3dpp.BackBufferWidth = ScreenWidth;
	d3dpp.BackBufferHeight = ScreenHeight;

	HRESULT hr = pDevice->Reset(&d3dpp);
	if (FAILED(hr))
	{
		std::cerr << "[ERROR] Device reset failed: " << std::hex << hr << std::endl;
		return false;
	}

	ImGui_ImplDX9_CreateDeviceObjects();
	return true;
}

/// <summary>
/// Window procedure function for handling Windows messages in a Win32 application, with integration for ImGui input processing.
/// </summary>
/// <param name="hWnd">Handle to the window receiving the message.</param>
/// <param name="msg">The message identifier specifying the type of event.</param>
/// <param name="wParam">Additional message information, usage depends on the message.</param>
/// <param name="lParam">Additional message information, usage depends on the message.</param>
/// <returns>A result code (LRESULT) indicating how the message was processed. Returns TRUE if ImGui handled the message, 0 for handled messages, or the result of DefWindowProcW for default processing.</returns>
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) return 0;
		ScreenWidth = (UINT)LOWORD(lParam);
		ScreenHeight = (UINT)HIWORD(lParam);
		return 0;

	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
