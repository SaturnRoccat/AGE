#include "Block.hpp"
#include <AgeAPI/internal/Addon/Addon.hpp>

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
			auto err = it->second->MergeDoublePush(addon, this, component);
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
	Block& Block::setGeometryInternal(Backend::Rp::Geometry&& geo)
	{
		mGeo = std::move(geo);
		// FIXME: Make it work :sob:
		return *this;
	}
	ErrorString Block::handleComponentRedirects(std::unique_ptr<Components::BlockComponentBase> component, NonOwningPtr<Addon> addon, bool override)
	{
		// FIXME: NO VERSION CHECKING HERE NEEDS TO BE ADDED

		auto it = mRedirectStore->find(component->GetComponentID().GetFullNamespace());
		if (it != mRedirectStore->end() && !override && !component->CanBeDoublePushed())
			return ErrorString("Component already exists and cannot be overridden");
		else if (it != mRedirectStore->end() && component->CanBeDoublePushed())
		{
			auto range = mRedirectStore->equal_range(component->GetComponentID().GetFullNamespace());
			for (auto sub_it = range.first; sub_it != range.second; ++sub_it)
			{
				
			}
		}
	}
}