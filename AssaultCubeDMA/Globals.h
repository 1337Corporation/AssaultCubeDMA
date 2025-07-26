#include <cstdint>

constexpr auto PROCESS_NAME = "ac_client.exe";

constexpr uintptr_t LocalPlayerOffset 		= 0x58AC00;
constexpr uintptr_t PlayersCountOffset		= 0x58AC0C;
constexpr uintptr_t PlayersListOffset		= 0x58AC04;
constexpr uintptr_t ViewMatrixOffset		= 0x57DFD0;
constexpr uintptr_t CurrentFrameOffset		= 0x57F10C;
constexpr uintptr_t VisibilityCheckOffset	= 0x58A918;

