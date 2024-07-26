#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeAPI::Backend
{
	ErrorString Permutation::WriteToJson(JsonProxy proxy, NonOwningPtr<Addon> addon, NonOwningPtr<AddonFeatures::Block> blk) const
	{
		rapidjson::Value condition(this->mCondition, proxy.mAllocator);
		proxy.mWriteLoc.AddMember("condition", condition, proxy.mAllocator);
		rapidjson::Value components(rapidjson::kObjectType);
		for (const auto& [namespace_, component] : this->mComponents)
		{
			rapidjson::Value key(namespace_, proxy.mAllocator);
			rapidjson::Value value(rapidjson::kObjectType);
			JsonProxy NewProxy(value, proxy.mAllocator);
			auto err = component->WriteToJson(addon, NewProxy, blk);
			if (err.ContainsError())
				return err;
			if (!component->IsTransient())
				components.AddMember(key, value, proxy.mAllocator);
		}
		proxy.mWriteLoc.AddMember("components", components, proxy.mAllocator);
		return ErrorString();
	}
}
