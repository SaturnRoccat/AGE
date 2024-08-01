#include "Block.hpp"
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <absl/log/log.h>
#include <absl/log/check.h>
#include <AgeData/BlockComponents.hpp>
#include <absl/log/flags.h>

#include <ranges>
namespace AgeAPI::AddonFeatures
{
	Block::Block(const Block& other)
	{
		mComponents.clear();
		for (auto& component : other.mComponents)
			mComponents[component.first] = std::unique_ptr<Components::BlockComponentBase>(component.second->Clone());
		for (auto& permutation : other.mPermutations)
		{
			auto perm = *permutation.second.get();
			mPermutations[permutation.first] = std::make_unique<Backend::Permutation>(std::move(perm));
		}
		for (auto& trait : other.mTraits)
			mTraits[trait.first] = std::unique_ptr<Backend::Bp::TraitBase>(trait.second->Clone());
		for (auto& state : other.mStates)
			mStates[state.first] = std::unique_ptr<Backend::AState>(state.second->Clone());
		mIdentifier = other.mIdentifier;
		mTextures = other.mTextures;
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
		for (auto& permutation : other.mPermutations)
		{
			auto perm = *permutation.second.get();
			mPermutations[permutation.first] = std::make_unique<Backend::Permutation>(std::move(perm));
		}
		for (auto& trait : other.mTraits)
			mTraits[trait.first] = std::unique_ptr<Backend::Bp::TraitBase>(trait.second->Clone());
		for (auto& state : other.mStates)
			mStates[state.first] = std::unique_ptr<Backend::AState>(state.second->Clone());
		mIdentifier = other.mIdentifier;
		mFormatVersion = other.mFormatVersion;
		mFormatVersion = other.mFormatVersion;
		mTextures = other.mTextures;
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
	Block&& Block::AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override, NonOwningPtr<Addon> addon)
	{
		DLOG(INFO) << std::format("Adding component to {}: {}", this->mIdentifier.GetFullNamespace(), component->GetComponentID().GetFullNamespace());
		DLOG_IF(INFO, addon) << std::format("Using user privided addon!");
		if (!addon)
			addon = Addon::GetStaticInstance();
		if (mRedirectStore)
		{
			auto err = handleComponentRedirects(std::move(component), addon, override);
			if (!err)
				throw std::runtime_error(err.GetAsString());
		}

		// FIXME: NO VERSION CHECKING HERE NEEDS TO BE ADDED
		auto it = mComponents.find(component->GetComponentID().GetFullNamespace());
		if (!override && it != mComponents.end() && !component->CanBeDoublePushed())
			throw ErrorString("Component already exists and cannot be overridden");
		else if (it != mComponents.end() && component->CanBeDoublePushed())
		{
			auto err = it->second->MergeDoublePush(addon, component);
			if (!err)
				throw std::runtime_error(err.GetAsString());
			return std::move(*this);
		}
		auto err = component->OnComponentAdded(addon, this);
		if (!err)
			throw std::runtime_error(err.GetAsString());
		mComponents[component->GetComponentID().GetFullNamespace()] = std::move(component);
		return std::move(*this);
	}
	Block&& Block::RemoveComponent(const std::string& componentName, NonOwningPtr<Addon> addon)
	{
		if (!addon)
			addon = Addon::GetStaticInstance();
		auto it = mComponents.find(componentName);
		if (it == mComponents.end())
			return std::move(*this);
		auto err = it->second->OnComponentRemoved(addon, this);
		if (!err)
			throw std::runtime_error(err.GetAsString());
		return std::move(*this);
	}
	Block&& Block::AddPermutation(std::unique_ptr<Backend::Permutation> permutation, bool override, NonOwningPtr<Addon> addon)
	{
		if (!addon)
			addon = Addon::GetStaticInstance();
		return addPermutationInternal(std::move(permutation), override, addon);
	}
	Block&& Block::AddTrait(std::unique_ptr<Backend::Bp::TraitBase> trait, bool override)
	{
		auto err = addTraitInternal(std::move(trait), override);
		if (!err)
			throw std::runtime_error(err.GetAsString());
		return std::move(*this);
	}
	Block&& Block::AddState(std::unique_ptr<Backend::AState> state, bool override)
	{
		auto err = addStateInternal(std::move(state), override);
		if (!err)
			throw std::runtime_error(err.GetAsString());
		return std::move(*this);
	}
	Block&& Block::setGeometryInternal(Backend::Rp::Geometry&& geo)
	{
		mGeo = std::move(geo);
		// FIXME: Make it work :sob:
		return std::move(*this);
	}
	ErrorString Block::handleComponentRedirects(std::unique_ptr<Components::BlockComponentBase> component, NonOwningPtr<Addon> addon, bool override)
	{
		// FIXME: NO VERSION CHECKING HERE NEEDS TO BE ADDED
		DLOG(INFO) << std::format("Adding component via redirection: {}", component->GetComponentID().GetFullNamespace());
		auto it = mRedirectStore->find(component->GetComponentID().GetFullNamespace());
		if (it != mRedirectStore->end() && !override && !component->CanBeDoublePushed())
			throw ErrorString("Component already exists and cannot be overridden");
		else if (it != mRedirectStore->end() && component->CanBeDoublePushed())
		{
			auto err = it->second->MergeDoublePush(addon, component);
			if (!err)
				throw std::runtime_error(err.GetAsString());
			return {};
		}
		auto err = component->OnComponentAdded(addon, this);
		if (!err)
			throw std::runtime_error(err.GetAsString());
		mRedirectStore->insert({ component->GetComponentID().GetFullNamespace(), std::move(component) });
		return {};
	}
	Block&& Block::addPermutationInternal(std::unique_ptr<Backend::Permutation> permutation, bool override, NonOwningPtr<Addon> addon)
	{
		DLOG(INFO) << std::format("Adding permutation: {}", permutation->GetCondition());
		ScopedToggle toggle(mEnableRewriteRedirection); // This should turn redirection on then at the end of the scope it should turn it off
		mRedirectStore = &permutation->mComponents;
		if (!override && mPermutations.contains(permutation->GetCondition()))
			throw ErrorString("Permutation already exists and cannot be overridden. Enable the override flag if it is needed.");
		for (auto& [name, component] : permutation->mComponents)
		{
			DLOG(INFO) << std::format("Adding component to permutation: {}", name);
			auto err = component->OnComponentAdded(addon, this);
			if (!err)
				throw std::runtime_error(err.GetAsString());
		}
		mPermutations[permutation->mCondition] = std::move(permutation);
		mRedirectStore = nullptr;
		return std::move(*this);
	}
	ErrorString Block::addTraitInternal(std::unique_ptr<Backend::Bp::TraitBase> trait, bool override)
	{
		DLOG(INFO) << std::format("Adding trait: {}", trait->GetTraitId().GetFullNamespace());
		auto it = mTraits.find(trait->GetTraitId().GetFullNamespace());
		if (!override && it != mTraits.end())
		{
			it->second->Merge(*trait);
			return {};
		}
		mTraits[trait->GetTraitId().GetFullNamespace()] = std::move(trait);
		return {};
	}
	ErrorString Block::addStateInternal(std::unique_ptr<Backend::AState> state, bool override)
	{
		DLOG(INFO) << std::format("Adding state: {}", state->GetStateId().GetFullNamespace());
		if (!override && mStates.contains(state->GetStateId().GetFullNamespace()))
			throw ErrorString("State already exists and cannot be overridden. Enable the override flag if it is needed.");
		mStates[state->GetStateId().GetFullNamespace()] = std::move(state);
		return {};
	}

	void Block::writeDescription(JsonProxy proxy, NonOwningPtr<Addon> addon)
	{
		rapidjson::Value descriptionObj(rapidjson::kObjectType);
		auto& json = descriptionObj;
		auto& alloc = proxy.mAllocator;

		json.AddMember("identifier", mIdentifier.GetFullNamespace(), alloc);
		mMenuCategory.WriteToJson({ json, alloc });

		// Handle States
		rapidjson::Value states(rapidjson::kObjectType);
		for (auto& [name, state] : mStates)
			state->WriteState(states, alloc);
		json.AddMember("states", states, alloc);

		// Handle Traits
		rapidjson::Value traits(rapidjson::kObjectType);
		for (auto& [name, trait] : mTraits)
		{
			rapidjson::Value traitObj(rapidjson::kObjectType);
			trait->WriteToJson({ traitObj, alloc }, addon);
			rapidjson::ValueWriteWithKey<rapidjson::Value>::WriteToJsonValue(trait->GetTraitId().GetFullNamespace(), traitObj, traits, alloc);
		}
		json.AddMember("traits", traits, alloc);

		proxy.mWriteLoc.AddMember("description", json, alloc);
	}

	void Block::writeComponents(JsonProxy proxy, NonOwningPtr<Addon> addon)
	{
		rapidjson::Value componentsObj(rapidjson::kObjectType);
		auto& json = componentsObj;
		auto& alloc = proxy.mAllocator;

		if (!mComponents.contains("minecraft:geometry"))
		{
			AddComponent(
				AgeData::BlockComponents::Geometry{ mGeo.GetGeoName() }
			);

		}
		if (!mComponents.contains("minecraft:material_instances"))
		{
			using namespace Backend::Rp;
			absl::InlinedVector<MaterialInstance::MaterialInstanceElement, 1> materials;
			for (auto& texture : mTextures)
			{
				materials.push_back(
					MaterialInstance::MaterialInstanceElement{
						texture.mTextureAlias,
						texture.mSide,
					}
					);
			}
			AddComponent(
				AgeData::BlockComponents::MaterialInstances{std::move(materials)}
			);
		}
		for (auto& [ComponentName, Component] : mComponents)
		{
			rapidjson::Value componentObj(rapidjson::kObjectType);
			auto err = Component->WriteToJson(addon, { componentObj, alloc }, this);
			if (!err)
				throw std::runtime_error(err.GetAsString());
			if (Component->IsTransient()) // Transient components are not written to the final json
				continue;
			rapidjson::ValueWriteWithKey<rapidjson::Value>::WriteToJsonValue(ComponentName, componentObj, componentsObj, alloc);
		}
		proxy.mWriteLoc.AddMember("components", componentsObj, alloc);
	}

	void Block::writePermutations(JsonProxy proxy, NonOwningPtr<Addon> addon)
	{
		rapidjson::Value permutationsObj(rapidjson::kArrayType);
		auto& json = permutationsObj;
		auto& alloc = proxy.mAllocator;
		for (auto& [condition, permutation] : mPermutations)
		{
			ScopedToggle toggle(mEnableRewriteRedirection);
			mRedirectStore = &permutation->mComponents;
			rapidjson::Value permutationObj(rapidjson::kObjectType);
			auto err = permutation->WriteToJson({ permutationObj, alloc }, addon, this);
			if (!err)
				throw std::runtime_error(err.GetAsString());
			permutationsObj.PushBack(permutationObj, alloc);
		}
		mRedirectStore = nullptr;
		proxy.mWriteLoc.AddMember("permutations", permutationsObj, alloc);
	}

	void Block::WriteToValue(JsonProxy proxy, NonOwningPtr<Addon> addon)
	{
		if (!addon) [[likely]]
			addon = Addon::GetStaticInstance();
		auto& [json, alloc] = proxy;
		json.AddMember("format_version", mFormatVersion.GetString(), alloc);
		rapidjson::Value minecraftBlock(rapidjson::kObjectType);
		writeDescription(proxy.Derive(minecraftBlock), addon);
		writeComponents(proxy.Derive(minecraftBlock), addon);
		writePermutations(proxy.Derive(minecraftBlock), addon);
		json.AddMember("minecraft:block", minecraftBlock, alloc); // So i dont forget to add it
	}
}