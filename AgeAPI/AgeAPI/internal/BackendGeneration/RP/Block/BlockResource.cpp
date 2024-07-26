#include "BlockResource.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{

    const std::string& GetTextureSideAsString(TextureSide side)
	{
		static const std::array<std::string, 6> sides{"up", "down", "north", "east", "south", "west"};
		return sides[ToUnderlying(side)];
	}
}
