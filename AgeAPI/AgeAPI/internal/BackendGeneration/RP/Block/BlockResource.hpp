#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
#include <filesystem>
#include <AgeAPI/internal/BackendGeneration/RP/ModelManager.hpp>
#include <variant>
#include <string>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <absl/container/inlined_vector.h>

namespace AgeAPI::Backend::Rp
{
	enum class TextureSide : uint8_t
	{
		up,
		down,
		north,
		east,
		south,
		west,
		all
	};
	struct BlockResourceElement
	{
		std::optional<Texture> mTexture{};
		std::string mTextureAlias{};
		std::string mPathFromBase{};
		TextureSide mSide{};
		BlockResourceElement() = default;

		template<typename a1, typename a2, typename a3 = a2>
			requires std::is_constructible_v<Texture, a1&&>&& std::is_constructible_v<std::string, a2&&>&& std::is_constructible_v<std::string, a3&&>
		BlockResourceElement(
			a1&& texture,
			a2&& textureAlias,
			a3&& pathFromBase = "",
			TextureSide side = TextureSide::all
		) : mTexture(std::forward<a1>(texture)), mTextureAlias(std::forward<a2>(textureAlias)), mPathFromBase(std::forward<a3>(pathFromBase)), mSide(side)
		{
			if (mPathFromBase.empty())
				mPathFromBase = mTextureAlias;
		}
		template<typename a1, typename a2>
			requires std::is_constructible_v<std::string, a1&&>&& std::is_constructible_v<std::string, a2&&>
		BlockResourceElement(
			a1&& textureAlias,
			a2&& pathFromBase = "",
			TextureSide side = TextureSide::all
		) : mTextureAlias(std::forward<a1>(textureAlias)), mPathFromBase(std::forward<a2>(pathFromBase)), mSide(side)
		{
			if (mPathFromBase.empty())
				mPathFromBase = mTextureAlias;
		}
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
			MaterialSide mSide{ MaterialSide::all };
			MaterialInstanceElement() = default;
			MaterialInstanceElement(const std::string& textureName, MaterialSide side = MaterialSide::all, bool faceDimming = true, bool ambientOcclusion = true)
			: mTextureName(textureName), mFaceDimming(faceDimming), mAmbientOcclusion(ambientOcclusion), mSide(side) {}
		};

		MaterialInstance() = default;
		MaterialInstance(const MaterialInstanceElement& element, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials({ element }) {}
		MaterialInstance(MaterialInstanceElement&& element, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials({ std::move(element) }) {}
		MaterialInstance(const absl::InlinedVector<MaterialInstanceElement, 1>& elements, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials(elements) {}
		MaterialInstance(absl::InlinedVector<MaterialInstanceElement, 1>&& elements, RenderMethod renderMethod = RenderMethod::opaque)
			: mRenderMethod(renderMethod), mMaterials(std::move(elements)) {}
		MaterialInstance(RenderMethod renderMethod) : mRenderMethod(renderMethod) {}

		absl::InlinedVector<MaterialInstanceElement, 1> mMaterials{};
	};

}