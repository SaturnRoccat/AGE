#include "TerrainTextures.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>



namespace AgeAPI::Backend::Rp
{
	TerrainTexture::TerrainTexture(TerrainTexture&& other) noexcept
	{
		mMipMapLevels = other.mMipMapLevels;
		mPadding = other.mPadding;
		mAtlasName = std::move(other.mAtlasName);
		mResourcePackName = std::move(other.mResourcePackName);
		mTextureData = std::move(other.mTextureData);
	}
}