#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <AgeAPI/internal/Addon/Addon.hpp>
namespace AgeAPI::Backend
{
	ErrorString Permutation::WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const
	{
		return ErrorString();
	}
	Permutation&& Permutation::AddComponent(std::unique_ptr<Components::BlockComponentBase> component, bool override, NonOwningPtr<Addon> addon)
	{
		if (!addon)
			addon = Addon::GetStaticInstance();

		auto it = mComponents.find(component->GetComponentID().GetFullNamespace());
		if (!override && it != mComponents.end() && !component->CanBeDoublePushed())
			throw ErrorString("Component already exists");
		else if (it != mComponents.end() && component->CanBeDoublePushed())
		{
			auto err = it->second->MergeDoublePush(addon, component);
			if (!err)
				throw std::runtime_error(err.GetAsString());
			return std::move(*this);
		}
		mComponents.emplace(component->GetComponentID().GetFullNamespace(), std::move(component)); 
		// We dont invoke the OnAddComponent since a component may rely on having access to the block and since we dont know what block the permutation is added too we cant pass it
		// So we just add the component shallowly and then when the permutation is added to a block we call OnAddComponent on all components in the permutation 
		return std::move(*this);
	}
}
