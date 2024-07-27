#include "Block.hpp"
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <absl/log/log.h>
#include <absl/log/flags.h>

namespace AgeAPI::AddonFeatures
{
	Block::Block(const Block& other)
	{
		for (auto& component : other.mComponents)
			mComponents[component.first] = std::unique_ptr<Components::BlockComponentBase>(component.second->Clone());
		mIdentifier = other.mIdentifier;
		mFormatVersion = other.mFormatVersion;
		mFormatVersion = other.mFormatVersion;
		mShowInCommands = other.mShowInCommands;
		
	}
	Block& Block::operator=(const Block& other)
	{
		if (this == &other)
			return *this;

		mComponents.clear();
		for (auto& component : other.mComponents)
			mComponents[component.first] = std::unique_ptr<Components::BlockComponentBase>(component.second->Clone());
		mIdentifier = other.mIdentifier;
		mFormatVersion = other.mFormatVersion;
		mFormatVersion = other.mFormatVersion;
		mShowInCommands = other.mShowInCommands;
		return *this;
	}
	std::expected<Block, ErrorString> Block::MakeBlock(const Identifier& identifier, SemanticVersion formatVersion, const MenuCategory& catagory, bool showInCommands)
	{
		Block block;
		if (!identifier.Validate())
			return std::unexpected("invalid identifier");
		block.mIdentifier = identifier;
		block.mFormatVersion = formatVersion;
		block.mShowInCommands = showInCommands;

		return block;
	}
	ReferenceExpected<Block, ErrorString> Block::AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override, NonOwningPtr<Addon> addon)
	{
		DLOG(INFO) << std::format("Adding component to {}: {}", this->mIdentifier.GetFullNamespace(), component->GetComponentID().GetFullNamespace());
		DLOG_IF(INFO, addon) << std::format("Using user privided addon!");
		if (!addon)
			addon = Addon::GetStaticInstance();
		if (mRedirectStore)
			return handleComponentRedirects(std::move(component), addon, override);

		// FIXME: NO VERSION CHECKING HERE NEEDS TO BE ADDED
		auto it = mComponents.find(component->GetComponentID().GetFullNamespace());
		if (!override && it != mComponents.end() && !component->CanBeDoublePushed())
			return ErrorString("Component already exists and cannot be overridden");
		else if (it != mComponents.end() && component->CanBeDoublePushed())
		{
			auto err = it->second->MergeDoublePush(addon, component);
			if (!err)
				return err;
			return *this;
		}
		auto err = component->OnComponentAdded(addon, this);
		if (!err)
			return err;
		mComponents[component->GetComponentID().GetFullNamespace()] = std::move(component);
		return *this;
	}
	ReferenceExpected<Block, ErrorString> Block::RemoveComponent(const std::string& componentName, NonOwningPtr<Addon> addon)
	{
		if (!addon)
			addon = Addon::GetStaticInstance();
		auto it = mComponents.find(componentName);
		if (it == mComponents.end())
			return *this;
		auto err = it->second->OnComponentRemoved(addon, this);
		if (!err)
			return err;
		return *this;
	}
	ReferenceExpected<Block, ErrorString> Block::AddPermutation(std::unique_ptr<Backend::Permutation> permutation, bool override, NonOwningPtr<Addon> addon)
	{
		if (!addon)
			addon = Addon::GetStaticInstance();
		return addPermutationInternal(std::move(permutation), override, addon);
	}
	ReferenceExpected<Block, ErrorString> Block::AddTrait(std::unique_ptr<Backend::Bp::TraitBase> trait, bool override)
	{
		auto err =  addTraitInternal(std::move(trait), override);
		if (!err)
			return err;
		return *this;
	}
	ReferenceExpected<Block, ErrorString> Block::AddState(std::unique_ptr<Backend::AState> state, bool override)
	{
		auto err = addStateInternal(std::move(state), override);
		if (!err)
			return err;
		return *this;
	}
	Block& Block::setGeometryInternal(Backend::Rp::Geometry&& geo)
	{
		mGeo = std::move(geo);
		// FIXME: Make it work :sob:
		return *this;
	}
	ErrorString Block::handleComponentRedirects(std::unique_ptr<Components::BlockComponentBase> component, NonOwningPtr<Addon> addon, bool override)
	{
		// FIXME: NO VERSION CHECKING HERE NEEDS TO BE ADDED
		DLOG(INFO) << std::format("Adding component via redirection: {}", component->GetComponentID().GetFullNamespace());
		auto it = mRedirectStore->find(component->GetComponentID().GetFullNamespace());
		if (it != mRedirectStore->end() && !override && !component->CanBeDoublePushed())
			return ErrorString("Component already exists and cannot be overridden");
		else if (it != mRedirectStore->end() && component->CanBeDoublePushed())
		{
			auto err = it->second->MergeDoublePush(addon, component);
			if (!err)
				return err;
			return {};
		}
		auto err = component->OnComponentAdded(addon, this);
		if (!err)
			return err;
		mRedirectStore->insert({ component->GetComponentID().GetFullNamespace(), std::move(component) });
		return {};
	}
	ReferenceExpected<Block, ErrorString> Block::addPermutationInternal(std::unique_ptr<Backend::Permutation> permutation, bool override, NonOwningPtr<Addon> addon)
	{
		DLOG(INFO) << std::format("Adding permutation: {}", permutation->GetCondition());
		ScopedToggle toggle(mEnableRewriteRedirection); // This should turn redirection on then at the end of the scope it should turn it off
		mRedirectStore = &permutation->mComponents;
		if (!override && mPermutations.contains(permutation->GetCondition()))
			return ErrorString("Permutation already exists and cannot be overridden. Enable the override flag if it is needed.");
		for (auto& [name, component] : permutation->mComponents)
		{
			auto err = component->OnComponentAdded(addon, this);
			if (!err)
				return err;
		}
		mPermutations[permutation->GetCondition()] = std::move(permutation);
		mRedirectStore = nullptr;
		return *this;
	}
	ErrorString Block::addTraitInternal(std::unique_ptr<Backend::Bp::TraitBase> trait, bool override)
	{
		DLOG(INFO) << std::format("Adding trait: {}", trait->GetTraitId().GetFullNamespace());
		if (!override && mTraits.contains(trait->GetTraitId().GetFullNamespace()))
			return ErrorString("Trait already exists and cannot be overridden. Enable the override flag if it is needed.");
		mTraits[trait->GetTraitId().GetFullNamespace()] = std::move(trait);
		return {};
	}
	ErrorString Block::addStateInternal(std::unique_ptr<Backend::AState> state, bool override)
	{
		DLOG(INFO) << std::format("Adding state: {}", state->GetStateId().GetFullNamespace());
		if (!override && mStates.contains(state->GetStateId().GetFullNamespace()))
			return ErrorString("State already exists and cannot be overridden. Enable the override flag if it is needed.");
		mStates[state->GetStateId().GetFullNamespace()] = std::move(state);
		return {};
	}
}