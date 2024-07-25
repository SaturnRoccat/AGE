#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeAPI/internal/BackendGeneration/States/States.hpp>
#include <vector>
#include <unordered_map>
#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>
namespace AgeAPI::AddonFeatures
{
	class IBlock;
}
namespace AgeAPI::Backend::Bp
{

	class BlockBehaviour
	{
	public:
		BlockBehaviour() = default;
		BlockBehaviour(const BlockBehaviour&) = delete;
		BlockBehaviour(BlockBehaviour&&) = default;
		BlockBehaviour(
			const Identifier& blockIdentifier,
			const SemanticVersion& formatVersion
		) : mBlockIdentifier(blockIdentifier), mFormatVersion(formatVersion) {}

		ErrorString AddBlockComponentC(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase>& component);
		ErrorString AddBlockComponent(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase> component)
		{
			return AddBlockComponentC(addon, component);
		}
		// WARNING: This is an internal function, this is not designed to be used outside of implementation of the API
		void RemoveBlockComponent(const std::string& componentName) { mBlockComponents.erase(componentName); }
		// WARNING: This is an internal function, this is not designed to be used outside of implementation of the API
		// This also bypasses version checking, so be careful when implementing the API
		void AddBlockComponentInternalC(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase>& component);
		void AddBlockComponentInternal(NonOwningPtr<Addon> addon, std::unique_ptr<Components::BlockComponentBase> component) { AddBlockComponentInternalC(addon, component); }
		void AddState(std::unique_ptr<AState> state) { mStates.emplace_back(std::move(state)); }
		void AddPermutation(Permutation&& permutation) { mPermutations.emplace_back(std::move(permutation)); }
		void SetIdentifier(const Identifier& identifier) { mBlockIdentifier = identifier; }
		void SetCategory(const MenuCategory& category) { mCategory = category; }

		const auto& GetBlockComponents() const { return mBlockComponents; }
		const auto& GetStates() const { return mStates; }
		const auto& GetPermutations() const { return mPermutations; }
		const auto& GetIdentifier() const { return mBlockIdentifier; }

		ErrorString BuildBlockBehaviourJson(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator) ;
		std::expected<rapidjson::Document, ErrorString> BuildBlockBehaviourDocument(NonOwningPtr<Addon> addon);
	private:
		ErrorString WriteComponents(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator) ;
		ErrorString WritePermutations(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator) ;
		void WriteStates(rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator) ;
	private:
		std::unordered_map<std::string, std::unique_ptr<Components::BlockComponentBase>> mBlockComponents{};
		std::vector<std::unique_ptr<AState>> mStates{};
		std::vector<Permutation> mPermutations{};
		MenuCategory mCategory{};
		// TODO: Add custom components and block traits and tags

		Identifier mBlockIdentifier{};
		SemanticVersion mFormatVersion{};
	};
}
 