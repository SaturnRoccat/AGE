#include <AgeAPI/internal/Utils/Direction.hpp>
#include <array>

namespace AgeAPI
{
	std::string GetDirectionAsString(AllDirections dir)
	{
		// Its actually faster to copy these strings since they are less t han 8 bytes and there is SSO so no heap allocation
		const static std::array<std::string, 6> directions = { "north", "east", "south", "west", "up", "down" };
		return directions[static_cast<int>(dir)];
	}
}