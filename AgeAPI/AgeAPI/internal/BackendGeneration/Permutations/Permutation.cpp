#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend
{
	ErrorString Permutation::WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const
	{
		return ErrorString();
	}
	ReferenceExpected<Permutation, ErrorString> Permutation::AddComponent(std::unique_ptr<Components::BlockComponentBase> component)
	{
		auto it = mComponents.find(component->GetComponentID().GetFullNamespace());
		if (it != mComponents.end() && !component->CanBeDoublePushed())
			return ErrorString("Component already exists");
		mComponents.insert({ component->GetComponentID().GetFullNamespace(), std::move(component) });
		return *this;
	}
}
