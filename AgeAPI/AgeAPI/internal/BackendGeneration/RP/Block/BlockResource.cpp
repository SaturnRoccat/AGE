#include "BlockResource.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{
    void BlockResource::AddTexture(TextureSide side, const BlockResourceElement& texture)
    {
        if (std::holds_alternative<BlockResourceElement>(mTextures))
        {
			auto textureStore = std::get<BlockResourceElement>(mTextures);
			mTextures = std::vector<std::pair<TextureSide, BlockResourceElement>>{ {TextureSide::ALL, textureStore}, {side, texture} };
		}
		else
		{
			auto& textureStore = std::get<std::vector<std::pair<TextureSide, BlockResourceElement>>>(mTextures);
			textureStore.push_back({ side, texture });
		}
    }
}
