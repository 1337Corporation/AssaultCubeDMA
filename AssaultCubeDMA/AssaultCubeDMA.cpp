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
}
