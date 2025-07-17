#include "Memory.h"
#include <iostream>

int main()
{
    if(!TargetProcess.Init("ac_client.exe"))
    {
        std::cout << "Failed to initialize process" << std::endl;
    }
}
