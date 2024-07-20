 #pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
#include <filesystem>
#include <AgeAPI/internal/BackendGeneration/RP/ModelManager.hpp>
#include <variant>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{
	struct BlockResourceElement
	{
		Texture mTexture{};
		std::string mTextureAlias{};
		BlockResourceElement() = default;
		BlockResourceElement(const Texture& texture, const std::string& textureAlias)
			: mTexture(texture), mTextureAlias(textureAlias) {}
		BlockResourceElement(Texture&& texture, std::string&& textureAlias) noexcept
			: mTexture(std::move(texture)), mTextureAlias(std::move(textureAlias)) {}
		BlockResourceElement(Texture&& texture, const std::string& textureAlias) noexcept
			: mTexture(std::move(texture)), mTextureAlias(textureAlias) {}
		BlockResourceElement& ReasignAlias(const std::string& alias) { mTextureAlias = alias; return *this; }
		BlockResourceElement& ReasignAlias(std::string&& alias) { mTextureAlias = std::move(alias); return *this; }



	};
	
	enum class TextureSide
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		ALL
	};

	class BlockResource
	{
	public:
		struct Geo
		{
			std::string mGeoName{};
			std::optional<Geometry> mGeometry{};
		};
		using MultiTextureStore = SmallVector<std::pair<TextureSide, BlockResourceElement>, 1>;
		using TextureStore = SmallVector<std::pair<TextureSide, BlockResourceElement>, 1>;
		using GeoType = std::optional<Geo>;
		using SoundType = std::string; // TODO: Change to a sound class or a sound ref after SoundManager is implemented
	private:
		TextureStore mTextures{};
		GeoType mGeo{};
		Identifier mBlockName{};
		SoundType mSound{};
		friend class ResourcePack;
	public:
		BlockResource() = default;

		BlockResource(const TextureStore& textures, const Identifier& blockName, const GeoType& geo = {}, const SoundType& sound = {})
			: mTextures(textures), mGeo(geo), mBlockName(blockName), mSound(sound) {}
		BlockResource(const BlockResourceElement& textures, const Identifier& blockName, const GeoType& geo = {}, const SoundType& sound = {})
			: mTextures({ {TextureSide::ALL, textures} }), mGeo(geo), mBlockName(blockName), mSound(sound) {}
		BlockResource(BlockResourceElement&& textures, const Identifier& blockName, const GeoType& geo = {}, const SoundType& sound = {})
			: mTextures({ {TextureSide::ALL, std::move(textures)} }), mGeo(geo), mBlockName(blockName), mSound(sound) {}
		BlockResource(TextureStore&& textures, Identifier&& blockName, GeoType&& geo = {}, SoundType&& sound = {})
			: mTextures(std::move(textures)), mGeo(std::move(geo)), mBlockName(std::move(blockName)), mSound(std::move(sound)) {}
		BlockResource(TextureStore&& textures, const Identifier& blockName, GeoType&& geo = {}, SoundType&& sound = {})
			: mTextures(std::move(textures)), mGeo(std::move(geo)), mBlockName(blockName), mSound(std::move(sound)) {}



		void SetOverallTexture(const BlockResourceElement& texture) { mTextures.clear(); mTextures.push_back({ TextureSide::ALL, texture }); }
		void SetOverallTexture(BlockResourceElement&& texture) { mTextures.clear(); mTextures.push_back({ TextureSide::ALL, std::move(texture) }); }

		void AddTexture(TextureSide side, const BlockResourceElement& texture);

		bool HasGeo() const { return mGeo.has_value(); }
		bool HoldsSingleTexture() const {
			return mTextures.IsSmall();
		}


		const GeoType::value_type& GetGeo() const { return mGeo.value(); }
		const TextureStore& GetTextures() const { return mTextures; }
		const BlockResourceElement& GetOverallTexture() const { return mTextures[0].second; }
		const MultiTextureStore& GetMultiTextures() const { return mTextures; }
	};

}