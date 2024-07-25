#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
#include <filesystem>
#include <AgeAPI/internal/BackendGeneration/RP/ModelManager.hpp>
#include <variant>
#include <string>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend::Rp
{
	enum class TextureSide : uint8_t
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		ALL,
		NONE
	};
	struct BlockResourceElement
	{
		Texture mTexture{};
		std::string mTextureAlias{};
		BlockResourceElement() = default;
		BlockResourceElement(const Texture& texture, const std::string& textureAlias) noexcept
			: mTexture(texture), mTextureAlias(textureAlias) {}
		BlockResourceElement(Texture&& texture, std::string&& textureAlias) noexcept
			: mTexture(std::move(texture)), mTextureAlias(std::move(textureAlias)) {}
		BlockResourceElement(Texture&& texture, const std::string& textureAlias) noexcept
			: mTexture(std::move(texture)), mTextureAlias(textureAlias) {}
		BlockResourceElement& ReasignAlias(const std::string& alias) { mTextureAlias = alias; return *this; }
		BlockResourceElement& ReasignAlias(std::string&& alias) { mTextureAlias = std::move(alias); return *this; }
	};
	const std::string& GetTextureSideAsString(TextureSide side);

	using MaterialSide = TextureSide;

	enum class RenderMethod
	{
		opaque,
		doubleSided,
		alphaTest,
		blend
	};

	struct MaterialInstance
	{
		RenderMethod mRenderMethod{ RenderMethod::opaque };
		std::string GetRenderMethodString() const
		{
			switch (mRenderMethod)
			{
			case RenderMethod::opaque:
				return "opaque";
			case RenderMethod::doubleSided:
				return "double_sided";
			case RenderMethod::alphaTest:
				return "alpha_test";
			case RenderMethod::blend:
				return "blend";
			default:
				return "opaque";
			}
		};
		struct MaterialInstanceElement
		{
			std::string mTextureName{};
			bool mFaceDimming{ true };
			bool mAmbientOcclusion{ true };
			MaterialSide mSide{ MaterialSide::ALL };
			MaterialInstanceElement() = default;
			MaterialInstanceElement(const std::string& textureName, MaterialSide side = MaterialSide::ALL, bool faceDimming = true, bool ambientOcclusion = true)
			: mTextureName(textureName), mFaceDimming(faceDimming), mAmbientOcclusion(ambientOcclusion), mSide(side) {}
		};

		MaterialInstance() = default;
		MaterialInstance(const MaterialInstanceElement& element, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials({ element }) {}
		MaterialInstance(MaterialInstanceElement&& element, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials({ std::move(element) }) {}
		MaterialInstance(const SmallVector<MaterialInstanceElement, 1>& elements, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials(elements) {}
		MaterialInstance(SmallVector<MaterialInstanceElement, 1>&& elements, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials(std::move(elements)) {}
		MaterialInstance(RenderMethod renderMethod) : mRenderMethod(renderMethod) {}

		SmallVector<MaterialInstanceElement, 1> mMaterials{};
	};

	class BlockResource
	{
	public:
		struct Geo
		{
			std::string mGeoName{};
			std::optional<Geometry> mGeometry{};
			Geo() = default;
			Geo(const Geometry& geometry) : mGeometry(geometry), mGeoName(geometry.GetGeoName()) {}
			Geo(Geometry&& geometry) : mGeometry(std::move(geometry)), mGeoName(mGeometry->GetGeoName()) {}
			Geo(const std::string& geoName) : mGeoName(geoName) {}
			Geo(std::string&& geoName) : mGeoName(std::move(geoName)) {}

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
		BlockResource(const BlockResourceElement& texture, const Identifier& blockName, const GeoType& geo = {}, const SoundType& sound = {})
			: mTextures({ {TextureSide::ALL, texture} }), mGeo(geo), mBlockName(blockName), mSound(sound) {}
		BlockResource(BlockResourceElement&& textures, const Identifier& blockName, const GeoType& geo = {}, const SoundType& sound = {})
			: mTextures({ {TextureSide::ALL, std::move(textures)} }), mGeo(geo), mBlockName(blockName), mSound(sound) {}
		BlockResource(TextureStore&& textures, Identifier&& blockName, GeoType&& geo = {}, SoundType&& sound = {})
			: mTextures(std::move(textures)), mGeo(std::move(geo)), mBlockName(std::move(blockName)), mSound(std::move(sound)) {}
		BlockResource(TextureStore&& textures, const Identifier& blockName, GeoType&& geo = {}, SoundType&& sound = {})
			: mTextures(std::move(textures)), mGeo(std::move(geo)), mBlockName(blockName), mSound(std::move(sound)) {}
		BlockResource(const Identifier& blockName) : mBlockName(blockName) {}



		BlockResource& SetOverallTexture(const BlockResourceElement& texture) { mTextures.clear(); mTextures.push_back({ TextureSide::ALL, texture }); return *this; }
		BlockResource& SetOverallTexture(BlockResourceElement&& texture) { mTextures.clear(); mTextures.push_back({ TextureSide::ALL, std::move(texture) }); return *this; }
		BlockResource& SetGeo(const GeoType& geo) { mGeo = geo; return *this; }
		BlockResource& SetGeo(GeoType&& geo) { mGeo = std::move(geo); return *this; }
		BlockResource& SetIdentifier(const Identifier& blockName) { mBlockName = blockName; return *this; }


		BlockResource& AddTexture(TextureSide side, const BlockResourceElement& texture);

		bool HasGeo() const { return mGeo.has_value(); }
		bool HoldsSingleTexture() const {
			return mTextures.IsSmall();
		}

		// WARNING: This function resets the texture state
		void WipeTextures() { mTextures.clear(); }
		void SwapTextures(TextureStore& textures) { std::swap(mTextures, textures); }

		const GeoType::value_type& GetGeo() const { return mGeo.value(); }
		const TextureStore& GetTextures() const { return mTextures; }
		const BlockResourceElement& GetOverallTexture() const { return mTextures[0].second; }
		const MultiTextureStore& GetMultiTextures() const { return mTextures; }
	};

}