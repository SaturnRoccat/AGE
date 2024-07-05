#include "TerrainTextures.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>



namespace AgeAPI::Backend::Rp
{
	inline TerrainTexture::TerrainTexture(TerrainTexture&& other) noexcept
	{
		mMipMapLevels = other.mMipMapLevels;
		mPadding = other.mPadding;
		mAtlasName = std::move(other.mAtlasName);
		mResourcePackName = std::move(other.mResourcePackName);
		mTextureData = std::move(other.mTextureData);
	}
	inline ErrorString TerrainTexture::AddTexture(const BlockResource& texture)
	{
		auto it = mTextureData.find(texture.GetTextureShortName());
		if (it != mTextureData.end())
			return "Texture with the same name already exists";
		mTextureData[texture.GetTextureShortName()] = texture;
		return ErrorString();
	}
	inline ErrorString TerrainTexture::AddTexture(BlockResource&& texture)
	{
		auto it = mTextureData.find(texture.GetTextureShortName());
		if (it != mTextureData.end())
			return "Texture with the same name already exists";
		mTextureData[texture.GetTextureShortName()] = std::move(texture);
		return ErrorString();
	}
	inline void TerrainTexture::Write(const std::filesystem::path& base) const
	{
		for (const auto& [name, texture] : mTextureData)
			texture.Write(base);
		return;
	}
	ErrorString TerrainTexture::WriteToJson(JsonProxy proxy) const
	{
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("resource_pack_name", mResourcePackName, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("texture_name", mAtlasName, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::ValueWriteWithKey<u8>::WriteToJsonValue("num_mip_levels", mMipMapLevels, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::ValueWriteWithKey<u8>::WriteToJsonValue("padding", mPadding, proxy.mWriteLoc, proxy.mAllocator);
		rapidjson::Value textureData(rapidjson::kObjectType);
		JsonProxy textureDataProxy{ textureData, proxy.mAllocator };
		for (auto& [name, texture] : mTextureData)
			texture.WriteToJson(textureDataProxy);
		proxy.mWriteLoc.AddMember("texture_data", textureData, proxy.mAllocator);
		return ErrorString();
	}
	inline std::expected<rapidjson::Document, ErrorString> TerrainTexture::WriteToDocument() const
	{
		rapidjson::Document doc;
		doc.SetObject();
		JsonProxy proxy{ doc, doc.GetAllocator() };
		auto err = WriteToJson(proxy);
		if (err.ContainsError())
			return std::unexpected(err);
		return doc;

	}
}