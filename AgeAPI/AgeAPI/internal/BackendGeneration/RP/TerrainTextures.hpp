#pragma once
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>


namespace AgeAPI::Backend::Rp
{
	class TerrainTexture
	{
	private:
		u8 mMipMapLevels{4};
		u8 mPadding{ 8 };
		std::string mAtlasName{"atlas.terrain"};
		std::string mResourcePackName{};
		std::unordered_map<std::string, BlockResource> mTextureData{};
	public:
		TerrainTexture() = default;
		TerrainTexture(const std::string& atlasName, const std::string& resourcePackName)
			: mAtlasName(atlasName), mResourcePackName(resourcePackName) {}
		TerrainTexture(const TerrainTexture& other) = delete;
		TerrainTexture(TerrainTexture&& other) noexcept
		{
			mMipMapLevels = other.mMipMapLevels;
			mPadding = other.mPadding;
			mAtlasName = std::move(other.mAtlasName);
			mResourcePackName = std::move(other.mResourcePackName);
			mTextureData = std::move(other.mTextureData);
		}

		const std::string& GetAtlasName() const { return mAtlasName; }
		const std::string& GetResourcePackName() const { return mResourcePackName; }
		const auto& GetTextureData() const { return mTextureData; }
		auto& GetTextureData() { return mTextureData; }

		void SetMipMapLevels(u8 mipMapLevels) { mMipMapLevels = mipMapLevels; }
		void SetPadding(u8 padding) { mPadding = padding; }

		ErrorString AddTexture(const BlockResource& texture)
		{
			auto it = mTextureData.find(texture.GetTextureShortName());
			if (it != mTextureData.end())
				return "Texture with the same name already exists";
			mTextureData[texture.GetTextureShortName()] = texture;
			return "";
		}

		ErrorString AddTexture(BlockResource&& texture)
		{
			auto it = mTextureData.find(texture.GetTextureShortName());
			if (it != mTextureData.end())
				return "Texture with the same name already exists";
			mTextureData[texture.GetTextureShortName()] = std::move(texture);
			return "";
		}

		void Write(const std::filesystem::path& base) const
		{
			for (const auto& [name, texture] : mTextureData)
				texture.Write(base);
			return;
		}
		ErrorString WriteToJson(JsonProxy proxy) const;
		std::expected<rapidjson::Document, ErrorString> WriteToDocument() const
		{
			rapidjson::Document doc;
			doc.SetObject();
			JsonProxy proxy{ doc, doc.GetAllocator() };
			auto err = WriteToJson(proxy);
			if (err.ContainsError())
				return std::unexpected(err);
			return doc;

		}
	};
}