#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/Traits/Traits.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Block/BlockResource.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Models/Models.hpp>
#include <optional>
#include <unordered_map>

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
#pragma region Add Components


		Block&& AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override = false, NonOwningPtr<Addon> addon = nullptr);
		template<Components::BlockComponent Component>
		Block&& AddComponent(Component&& component, bool override = false, NonOwningPtr<Addon> addon = nullptr)
		{
			return AddComponent(std::make_unique<Component>(std::forward<Component>(component)), override, addon);
		}
		Block&& RemoveComponent(const std::string& componentName, NonOwningPtr<Addon> addon = nullptr);
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddComponents(
			const Container& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
			{
				auto ptr = std::unique_ptr<Components::BlockComponentBase>(component->Clone());
				AddComponent(std::move(ptr), override, addon);
			}
			return std::move(*this);
		}
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddComponents(
			Container&& components,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& component : components)
				AddComponent(std::move(component), override, addon);
			return std::move(*this);
		}
#pragma endregion
#pragma region Misc


		template<typename a1> requires std::is_same_v<a1, Backend::Rp::BlockResourceElement>
		Block&& SetTexture(a1&& texture)
		{
			for (auto& existingText : mTextures)
			{
				if (existingText.mSide == texture.mSide)
				{
					existingText = std::forward<a1>(texture);
					return std::move(*this);
				}
			}
			mTextures.push_back(std::forward<a1>(texture));
			return std::move(*this);
		}
		/*
		* Let me explain what the hell the template magic is doing here because it's a bit of a doozy.
		* We are abusing perfect forwarding to allow the user to pass any mix of R and L values to the function without having to manually specify the type.
		*/
		template<typename a1, typename a2, typename a3 = a2>
			requires std::is_constructible_v<Backend::Rp::Texture, a1>&& std::is_same_v<std::string, a2>&& std::is_constructible_v<std::string, a3>
		Block&& SetTexture(a1&& texture, a2&& textureAlias, a3&& pathFromBase = "", Backend::Rp::TextureSide side = Backend::Rp::TextureSide::all)
		{
			if constexpr (std::is_same_v<a3, std::string>)
			{
				if (pathFromBase.empty())
					pathFromBase = a3(textureAlias);
			}

			return SetTexture(Backend::Rp::BlockResourceElement(std::forward<a1>(texture), std::forward<a2>(textureAlias), std::forward<a3>(pathFromBase), side));
		}

		template<typename a1> requires std::is_constructible_v<Identifier, a1>
		Block&& SetIdentifier(a1&& identifier)
		{
			Identifier temp = std::forward<a1>(identifier);
			if (!temp.Validate())
				return ErrorString("invalid identifier");
			mIdentifier = std::move(temp);
			return std::move(*this);

		}

		template<typename a1> requires std::is_constructible_v<Backend::Rp::Geometry, a1>
		Block&& SetGeometry(a1&& geo) {return setGeometryInternal(std::forward<a1>(geo));}
#pragma endregion
#pragma region Add Permutations


		Block&& AddPermutation(std::unique_ptr<Backend::Permutation> permutation, bool override = false, NonOwningPtr<Addon> addon = nullptr);
		template<typename T> requires std::is_constructible_v<Backend::Permutation, T&&>
		Block&& AddPermutation(T&& permutation, bool override = false, NonOwningPtr<Addon> addon = nullptr)
		{
			return AddPermutation(std::make_unique<Backend::Permutation>(std::forward<T>(permutation)), override, addon);
		}
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddPermutations(
			const Container& permutations,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& permutation : permutations)
			{
				auto ptr = std::unique_ptr<Backend::Permutation>(permutation->Clone());
				AddPermutation(std::move(ptr), override, addon);
			}
			return std::move(*this);
		}
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddPermutations(
			Container&& permutations,
			bool override = false,
			NonOwningPtr<Addon> addon = nullptr)
		{
			for (auto& permutation : permutations)
			{
				AddPermutation(std::move(permutation), override, addon);
			}
			return std::move(*this);
		}
		NonOwningPtr<Backend::Permutation> GetPermutation(const std::string& name) 
		{
			auto it = mPermutations.find(name);
			if (it == mPermutations.end())
				return nullptr;
			return it->second.get();
		
		}
#pragma endregion
#pragma region AddTraits


		Block&& AddTrait(std::unique_ptr<Backend::Bp::TraitBase> trait, bool override = false);
		template<typename T> requires std::is_base_of_v<Backend::Bp::TraitBase, T>
		Block&& AddTrait(T&& trait, bool override = false)
		{
			return AddTrait(std::make_unique<T>(std::forward<T>(trait)), override);
		}
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddTraits(
			const Container& traits,
			bool override = false)
		{
			for (auto& trait : traits)
			{
				auto ptr = std::unique_ptr<Backend::Bp::TraitBase>(trait->Clone());
				AddTrait(std::move(ptr), override);
			}
			return std::move(*this);
		}
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddTraits(
			Container&& traits,
			bool override = false)
		{
			for (auto& trait : traits)
			{
				AddTrait(std::move(trait), override);
			}
			return std::move(*this);
		}
#pragma endregion
#pragma region Add States
		Block&& AddState(std::unique_ptr<Backend::AState> state, bool override = false);
		template<typename T> requires std::is_constructible_v<Backend::AState, T&&>
		Block&& AddState(T&& state, bool override = false)
		{
			return AddState(std::make_unique<Backend::AState>(std::forward<T>(state)), override);
		}
		template<Backend::State State>
		Block&& AddState(State&& state)
		{
			return AddState(std::make_unique<State>(std::forward<State>(state)));
		}
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddStates(
			const Container& states,
			bool override = false)
		{
			for (auto& state : states)
			{
				auto ptr = std::unique_ptr<Backend::AState>(state->Clone());
				AddState(std::move(ptr), override);
			}
			return std::move(*this);
		}
		template<typename Container> requires std::ranges::range<Container>
		Block&& AddStates(
			Container&& states,
			bool override = false)
		{
			for (auto& state : states)
				AddState(std::move(state), override);
			return std::move(*this);
		}
#pragma endregion


		// WARNING: This invalidates/wipes the block this should ONLY EVER be used when used in method chaining
		Block Finalize() 
		{
			Block blk;
			std::swap(blk, *this);
			return blk;
		}

	private:
		Block&& setGeometryInternal(Backend::Rp::Geometry&& geo);
		ErrorString handleComponentRedirects(std::unique_ptr<Components::BlockComponentBase> component, NonOwningPtr<Addon> addon, bool override);
		Block&& addPermutationInternal(std::unique_ptr<Backend::Permutation> permutation, bool override, NonOwningPtr<Addon> addon);
		ErrorString addTraitInternal(std::unique_ptr<Backend::Bp::TraitBase> trait, bool override);
		ErrorString addStateInternal(std::unique_ptr<Backend::AState> state, bool override);

	private:
		using ComponentStore = absl::flat_hash_map<std::string, std::unique_ptr<Components::BlockComponentBase>>;
		using PermutationStore = absl::flat_hash_map<std::string, std::unique_ptr<Backend::Permutation>>;
		using StateStore = absl::flat_hash_map<std::string, std::unique_ptr<Backend::AState>>;
		using TraitStore = absl::flat_hash_map<std::string, std::unique_ptr<Backend::Bp::TraitBase>>;
		using TextureStore = absl::InlinedVector<Backend::Rp::BlockResourceElement, 1>;
		ComponentStore mComponents{};
		PermutationStore mPermutations{};
		TraitStore mTraits{};
		StateStore mStates{};
		TextureStore mTextures{};
		Identifier mIdentifier{};
		MenuCategory mMenuCategory{};
		SemanticVersion mFormatVersion{};
		bool mShowInCommands{};
		bool mEnableRewriteRedirection{ false };
		NonOwningPtr<ComponentStore> mRedirectStore{nullptr};
		Backend::Rp::Geometry mGeo{};
	};

	
}