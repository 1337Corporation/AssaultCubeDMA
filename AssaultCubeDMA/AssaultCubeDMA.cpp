#include "Memory.h"
#include "AssaultCubeDMA.hpp"
#include <iostream>

int main()
{
    if(!TargetProcess.Init(PROCESS_NAME))
    {
        std::cout << "Failed to initialize process" << std::endl;
    }

    size_t ModuleBaseAddress;

    ModuleBaseAddress = TargetProcess.GetBaseDaddy(PROCESS_NAME);
    if (!ModuleBaseAddress)
    {
        std::cout << "Failed to retrieve BaseAddress" << std::endl;
    }

    std::cout << std::hex << ModuleBaseAddress << std::endl;

    size_t LocalPlayerPtr;

    TargetProcess.Read(ModuleBaseAddress + 0x17E0A8, &LocalPlayerPtr, sizeof(LocalPlayerPtr));

    std::cout << std::hex << LocalPlayerPtr << std::endl;

    size_t health = 3333333;
    size_t ammo = 42;

    while (1)
    {
        TargetProcess.Write(LocalPlayerPtr + 0xEC, &health, sizeof(health));
        TargetProcess.Write(LocalPlayerPtr + 0x140, &ammo, sizeof(ammo));

    }
}
