#pragma once
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/TextureRegistry.hpp>


namespace AgeAPI::Backend::Rp
{


	class TerrainTexture
	{
	private:
		u8 mMipMapLevels{4};
		u8 mPadding{ 8 };
		std::string mAtlasName{"atlas.terrain"};
		std::string mResourcePackName{};
		TextureRegistry<BlockResourceElement> mTextureData{};
		friend class ResourcePack;
	private:
		TerrainTexture() = default;
		TerrainTexture(const std::string& atlasName, const std::string& resourcePackName)
			: mAtlasName(atlasName), mResourcePackName(resourcePackName) {}
		TerrainTexture(const TerrainTexture& other) = delete;
		TerrainTexture(TerrainTexture&& other) noexcept;
		TerrainTexture& operator=(const TerrainTexture& other) = delete;
		TerrainTexture& operator=(TerrainTexture&& other) noexcept  = default;
	public:

		TextureError BindBlockResourceElement(const BlockResourceElement& blkResourceElement)
		{
			return mTextureData.AddTexture(blkResourceElement.mTextureAlias, blkResourceElement);
		}



	};
}