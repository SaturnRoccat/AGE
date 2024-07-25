#include "BlockResource.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{
    BlockResource& BlockResource::AddTexture(TextureSide side, const BlockResourceElement& texture)
    {
        mTextures.push_back({side, texture});
        return *this;
    }

    const std::string& GetTextureSideAsString(TextureSide side)
	{
		static const std::array<std::string, 6> sides{"up", "down", "north", "south", "west", "east"};
		return sides[ToUnderlying(side)];
	}
}
