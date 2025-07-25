#include "Memory.h"
#include <iostream>
#include <thread>
#include <windows.h>
#include "Player.hpp"
#include "Globals.h"

int main()
{
    if(!TargetProcess.Init(PROCESS_NAME))
    {
        std::cout << "Failed to initialize process" << std::endl;
        while (1) {}
    }

    while (1)
    {
        Player player(LocalPlayerOffset, true);

        if (!player.IsValid())
        {
            std::cout << "Invalid player pointer" << std::endl;
        }

        player.SetHealth(111111);
        player.SetArmor(2424242);

        std::cout << player.GetHealth() << std::endl;
        std::cout << player.GetName() << std::endl;
		std::cout << player.GetArmor() << std::endl;

		Sleep(100);

        if (GetKeyState('A'))
            break;
    }
}
