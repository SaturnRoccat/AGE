#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend
{
	ErrorString Permutation::WriteToJson(JsonProxy& proxy, std::unique_ptr<Addon>& addon, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const
	{
		rapidjson::Value condition(this->mCondition, proxy.mAllocator);
		proxy.mWriteLoc.AddMember("condition", condition, proxy.mAllocator);
		rapidjson::Value components(rapidjson::kObjectType);
		for (const auto& component : this->mComponents)
		{
			rapidjson::Value key(component->GetComponentID().GetFullNamespace(), proxy.mAllocator);
			rapidjson::Value value(rapidjson::kObjectType);
			JsonProxy proxy(value, proxy.mAllocator);
			auto err = component->WriteToJson(addon, proxy, blk);
			if (err.ContainsError())
				return err;
			components.AddMember(key, value, proxy.mAllocator);
		}
		proxy.mWriteLoc.AddMember("components", components, proxy.mAllocator);
		return "";
	}
}
