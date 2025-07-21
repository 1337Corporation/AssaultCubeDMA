#include "Memory.h"
#include "AssaultCubeDMA.hpp"
#include <iostream>
#include <thread>

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
        TargetProcess.Write(LocalPlayerPtr + Offsets::Health, &health, sizeof(health));
        TargetProcess.Write(LocalPlayerPtr + Offsets::MachineGun, &ammo, sizeof(ammo));
        TargetProcess.Write(LocalPlayerPtr + Offsets::Pistol, &ammo, sizeof(ammo));

        TargetProcess.Read(LocalPlayerPtr + Offsets::PlayerCameraX, &cameraX, sizeof(cameraX));
        TargetProcess.Read(LocalPlayerPtr + Offsets::PlayerCameraY, &cameraY, sizeof(cameraY));
        std::cout << "\rPlayer CameraX -> " << cameraX << " | Player CameraY -> " << cameraY << std::flush;
    }
}
