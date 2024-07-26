#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Models/Models.hpp>
#include <optional>
#include <absl/container/inlined_vector.h>

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
		template<typename Container> requires std::ranges::range<Container>
		ReferenceExpected<Block, ErrorString> AddComponentsCopy(
			const Container& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto ptr = std::unique_ptr<Components::BlockComponentBase>(component->Clone());
				auto err = AddComponent(std::move(ptr), override, addon);
				if (!err)
					return err;
			}
			return *this;
		}
		template<typename Container> requires std::ranges::range<Container>
		ReferenceExpected<Block, ErrorString> AddComponents(
			Container&& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto err = AddComponent(std::move(component), override, addon);
				if (!err)
					return err;
			}
			return *this;
		}
		
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
		template<typename a1, typename a2, typename a3 = a2>
			requires std::is_constructible_v<Backend::Rp::Texture, a1>&& std::is_same_v<std::string, a2>&& std::is_constructible_v<std::string, a3>
		Block& SetTexture(a1&& texture, a2&& textureAlias, a3&& pathFromBase = "", Backend::Rp::TextureSide side = Backend::Rp::TextureSide::all)
		{
			if constexpr (std::is_same_v<a3, std::string>)
			{
				if (pathFromBase.empty())
					pathFromBase = a3(textureAlias);
			}

			return SetTexture(Backend::Rp::BlockResourceElement(std::forward<a1>(texture), std::forward<a2>(textureAlias), std::forward<a3>(pathFromBase), side));
		}

		template<typename a1> requires std::is_constructible_v<Identifier, a1>
		ReferenceExpected<Block, ErrorString> SetIdentifier(a1&& identifier)
		{
			Identifier temp = std::forward<a1>(identifier);
			if (!temp.Validate())
				return ErrorString("invalid identifier");
			mIdentifier = std::move(temp);
			return *this;

		}




	private:
		std::unordered_map<std::string, std::unique_ptr<Components::BlockComponentBase>> mComponents;
		absl::InlinedVector<Backend::Rp::BlockResourceElement, 1> mTextures{};
		Identifier mIdentifier{};
		SemanticVersion mFormatVersion{};
		MenuCategory mMenuCategory{};
		bool mShowInCommands{};



	};

	
}