#include "BlockResource.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{
    void BlockResource::AddTexture(TextureSide side, const BlockResourceElement& texture)
    {
        mTextures.push_back({side, texture});
    }
}
