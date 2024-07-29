#include "TerrainTextures.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <AgeAPI/internal/RapidJsonExtension/FileIO.hpp>



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

	ErrorString TerrainTexture::writeTextureData(const std::filesystem::path& base) const
	{
		std::filesystem::create_directories(base / "textures");
		for (auto& [_, Resource] : mTextureData)
			Resource.mTexture.FinalizeAndWrite(base / "textures" / "blocks" / std::format("{}.png", Resource.mTextureAlias));
		rapidjson::Document doc;
		doc.SetObject();
		doc.AddMember("padding", mPadding, doc.GetAllocator());
		doc.AddMember("texture_name", mAtlasName, doc.GetAllocator());
		doc.AddMember("resource_pack_name", mResourcePackName, doc.GetAllocator());
		doc.AddMember("num_mip_levels", mMipMapLevels, doc.GetAllocator());
		rapidjson::Value textures(rapidjson::kObjectType);
		for (auto& [name, _] : mTextureData)
		{
			rapidjson::Value textureInfo(rapidjson::kObjectType);
			textureInfo.AddMember("texture", std::format("textures/blocks/{}", name), doc.GetAllocator());
			rapidjson::Value key(name, doc.GetAllocator());
			textures.AddMember(key, textureInfo, doc.GetAllocator());
		}
		doc.AddMember("texture_data", textures, doc.GetAllocator());
		rapidjson::WriteJsonFile(doc, base / "textures" / "terrain_texture.json");

		return "";
	}

	TextureError TerrainTexture::BindBlockResourceElement(const BlockResourceElement& blkResourceElement, bool override)
	{
		//return mTextureData.AddTexture(blkResourceElement.GetTexture(), blkResourceElement, override);
		return TextureError::NONE;
	}
}