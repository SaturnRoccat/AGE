#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
#include <filesystem>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{
	class BlockResourceElement
	{
	public:
		BlockResourceElement() = default;
		BlockResourceElement(const std::string& outputPathFromBase, const Texture& blockTexture)
			: mOutputPathFromBase(outputPathFromBase), mBlockTexture(blockTexture) {}
		BlockResourceElement(const std::string& outputPathFromBase, Texture&& blockTexture)
			: mOutputPathFromBase(outputPathFromBase), mBlockTexture(std::move(blockTexture)) {}

		const std::string& GetOutputPathFromBase() const { return mOutputPathFromBase; }
		const Texture& GetBlockTexture() const { return mBlockTexture; }
		Texture& GetBlockTexture() { return mBlockTexture; }
		IVec2 GetTextureSize() const { return mBlockTexture.GetSize(); }
		i32 GetTextureWidth() const { return mBlockTexture.GetSize().x; }
		i32 GetTextureHeight() const { return mBlockTexture.GetSize().y; }

		void Write(const std::filesystem::path& base) const;
	private:
		std::string mOutputPathFromBase{};
		Texture mBlockTexture{};
	};
	class BlockResource
	{
	private:
		std::vector<BlockResourceElement> mTextures{};
		std::string mTextureShortName{};
	public:
		BlockResource() = default;
		BlockResource(const std::string& textureShortName, const std::vector<BlockResourceElement>& resources = {}) : mTextureShortName(textureShortName), mTextures(resources) {}

		const std::string& GetTextureShortName() const { return mTextureShortName; }
		const auto& GetTextures() const { return mTextures; }
		auto& GetTextures() { return mTextures; }

		void AddTexture(const BlockResourceElement& texture) { mTextures.push_back(texture); }
		void AddTexture(BlockResourceElement&& texture) { mTextures.push_back(std::move(texture)); }
		void Write(const std::filesystem::path& base) const { for (const auto& texture : mTextures) texture.Write(base); }

		ErrorString WriteToJson(JsonProxy proxy) const;
	};
}

namespace rapidjson
{

	template<>
	struct TypeTranslation<AgeAPI::Backend::Rp::BlockResourceElement, false>
	{
		static void WriteToJson(const AgeAPI::Backend::Rp::BlockResourceElement& value, rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator)
		{
			jsonValue.SetString(value.GetOutputPathFromBase(), allocator);
		}

		static AgeAPI::Backend::Rp::BlockResourceElement ReadFromJson(const rapidjson::Value& jsonValue)
		{
			return AgeAPI::Backend::Rp::BlockResourceElement(jsonValue.GetString(), {});
		}
	};
}