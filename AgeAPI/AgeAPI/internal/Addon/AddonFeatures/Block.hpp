#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Models/Models.hpp>
#include <optional>

namespace AgeAPI::AddonFeatures
{
	class Block
	{
	public:
		Block() = default;
		Block(const Block& other);
		Block(Block&& other) noexcept = default;
		Block& operator=(const Block& other);
		Block& operator=(Block&& other) noexcept = default;
		~Block() = default;

		static std::expected<Block, ErrorString> MakeBlock(
			const Identifier& identifier,
			SemanticVersion formatVersion = { 1, 21, 20 },
			const MenuCategory& catagory = {},
			bool showInCommands = true);
		ReferenceExpected<Block, ErrorString> AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override = false, NonOwningPtr<Addon> addon = nullptr);
		template<Components::BlockComponent Component>
		ReferenceExpected<Block, ErrorString> AddComponent(const Component& component, bool override = false, NonOwningPtr<Addon> addon = nullptr)
		{
			return AddComponent(std::make_unique<Component>(component), override, addon);
		}
		ReferenceExpected<Block, ErrorString> RemoveComponent(const std::string& componentName, NonOwningPtr<Addon> addon = nullptr);
		
		template<typename a1> requires std::is_same_v<a1, Backend::Rp::BlockResourceElement>
		Block& SetTexture(a1&& texture)
		{
			for (auto& existingText : mTextures)
			{
				if (existingText.mSide == texture.mSide)
				{
					existingText = std::forward<a1>(texture);
					return *this;
				}
			}
			mTextures.push_back(std::forward<a1>(texture));
			return *this;
		}
		/*
		* Let me explain what the hell the template magic is doing here because it's a bit of a doozy.
		* We are abusing perfect forwarding to allow the user to pass any mix of R and L values to the function without having to manually specify the type.
		*/
		template<typename a1, typename a2, typename a3>
			requires std::is_constructible_v<Backend::Rp::Texture, a1>&& std::is_constructible_v<std::string, a2>&& std::is_constructible_v<std::string, a3>
		Block& SetTexture(a1&& texture, a2&& textureAlias, a3&& pathFromBase = "", Backend::Rp::TextureSide side = Backend::Rp::TextureSide::all)
		{
			return SetTexture(Backend::Rp::BlockResourceElement(std::forward<a1>(texture), std::forward<a2>(textureAlias), std::forward<a3>(pathFromBase), side));
		}

		template<typename a1, typename a2>
			requires std::is_constructible_v<Backend::Rp::Texture, a1>&& std::is_constructible_v<std::string, a2>
		Block& SetTexture(a1&& texture, a2&& textureAlias, Backend::Rp::TextureSide side = Backend::Rp::TextureSide::all, const std::string& pathFromBase = "")
		{
			return SetTexture(Backend::Rp::BlockResourceElement(std::forward<a1>(texture), std::forward<a2>(textureAlias), pathFromBase, side));
		}



	private:
		std::unordered_map<std::string, std::unique_ptr<Components::BlockComponentBase>> mComponents;
		SmallVector<Backend::Rp::BlockResourceElement> mTextures{};
		Identifier mIdentifier{};
		SemanticVersion mFormatVersion{};
		MenuCategory mMenuCategory{};
		bool mShowInCommands{};



	};

	
}