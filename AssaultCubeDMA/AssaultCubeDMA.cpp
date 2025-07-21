#include "Memory.h"
#include "AssaultCubeDMA.hpp"
#include <iostream>
#include <thread>
#include <windows.h>

void moveCursor(int x, int y)
{
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    if(!TargetProcess.Init(PROCESS_NAME))
    {
        std::cout << "Failed to initialize process" << std::endl;
        while (1) {}
    }

    size_t ModuleBaseAddress;

    ModuleBaseAddress = TargetProcess.GetBaseDaddy(PROCESS_NAME);
    if (!ModuleBaseAddress)
    {
        std::cout << "Failed to retrieve BaseAddress" << std::endl;
        while (1) {}
    }

    std::cout << std::hex << ModuleBaseAddress << std::endl;

    size_t LocalPlayerPtr;

    TargetProcess.Read(ModuleBaseAddress + 0x17E0A8, &LocalPlayerPtr, sizeof(LocalPlayerPtr));

    std::cout << std::hex << LocalPlayerPtr << std::endl;

    size_t health = 3333333;
    size_t ammo = 42;

    float cameraX;
    float cameraY;

    float positionX;
    float positionY;
    float positionZ;

    system("cls");

    while (1)
    {
        // Infinite Ammo & Health ;)
        TargetProcess.Write(LocalPlayerPtr + Offsets::LocalPlayer::Health, &health, sizeof(health));
        TargetProcess.Write(LocalPlayerPtr + Offsets::Guns::PistolAmmo, &ammo, sizeof(ammo));
        TargetProcess.Write(LocalPlayerPtr + Offsets::Guns::MachineGunAmmo, &ammo, sizeof(ammo));


        // Reading Camera Values
        TargetProcess.Read(LocalPlayerPtr + Offsets::LocalPlayer::PlayerCameraX, &cameraX, sizeof(cameraX));
        moveCursor(0, 1);
        std::cout << "Player CameraX -> " << cameraX << std::endl;
        TargetProcess.Read(LocalPlayerPtr + Offsets::LocalPlayer::PlayerCameraY, &cameraY, sizeof(cameraY));
        moveCursor(0, 2);
        std::cout << "Player CameraY -> " << cameraY << std::endl;

        
        // Reading Position Values
        TargetProcess.Read(LocalPlayerPtr + Offsets::LocalPlayer::PlayerPosZ, &positionZ, sizeof(positionZ));
        moveCursor(0, 4);
        std::cout << "Player Position Z -> " << positionZ << std::endl;

        Sleep(100);
    }
}
