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
		TerrainTexture(TerrainTexture&& other) noexcept;

		const std::string& GetAtlasName() const { return mAtlasName; }
		const std::string& GetResourcePackName() const { return mResourcePackName; }
		const auto& GetTextureData() const { return mTextureData; }
		auto& GetTextureData() { return mTextureData; }

		void SetMipMapLevels(u8 mipMapLevels) { mMipMapLevels = mipMapLevels; }
		void SetPadding(u8 padding) { mPadding = padding; }

		ErrorString AddTexture(const BlockResource& texture);

		ErrorString AddTexture(BlockResource&& texture);

		void Write(const std::filesystem::path& base) const;
		ErrorString WriteToJson(JsonProxy proxy) const;
		std::expected<rapidjson::Document, ErrorString> WriteToDocument() const;
	};
}