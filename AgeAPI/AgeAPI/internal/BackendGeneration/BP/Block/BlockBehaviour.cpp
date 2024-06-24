#include "BlockBehaviour.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>

namespace AgeAPI::Backend::Bp
{
    ErrorString BlockBehaviour::BuildBlockBehaviourJson(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::ValueWriteWithKey<std::string>::WriteToJsonValue("format_version", this->mFormatVersion.GetString(), location, allocator);
        rapidjson::Value minecraftBlock(rapidjson::kObjectType);
        rapidjson::Value description(rapidjson::kObjectType);
        WriteComponents(addon, minecraftBlock, allocator);
        WritePermutations(addon, minecraftBlock, allocator);
        description.AddMember("identifier", this->mBlockIdentifier.GetFullNamespace(), allocator);
        WriteStates(description, allocator);
        minecraftBlock.AddMember("description", description, allocator);
        location.AddMember("minecraft:block", minecraftBlock, allocator);
        return "";
    }

    ErrorString BlockBehaviour::WriteComponents(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator) 
    {
        rapidjson::Value components(rapidjson::kObjectType);
        for (const auto& [key, component] : this->mBlockComponents)
		{
            rapidjson::Value key(key, allocator);
            rapidjson::Value value(rapidjson::kObjectType);
            JsonProxy proxy(value, allocator);
            auto err = component->WriteToJson(addon, proxy, this);
            if (err.ContainsError())
				return err;
            if (!component->IsTransient())
                components.AddMember(key, value, allocator);
		}
		location.AddMember("components", components, allocator);
        return "";
    }
    ErrorString BlockBehaviour::WritePermutations(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator)
    {
        if (this->mPermutations.empty())
			return "";
        rapidjson::Value permutations(rapidjson::kArrayType);
        for (const auto& permutation : this->mPermutations)
        {
            rapidjson::Value value(rapidjson::kObjectType);
			JsonProxy proxy(value, allocator);
            auto err = permutation.WriteToJson(proxy, addon, this);
			if (err.ContainsError())
                return err;
			permutations.PushBack(value, allocator);
        }
        location.AddMember("permutations", permutations, allocator);
        return "";
    }
    void BlockBehaviour::WriteStates(rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator)
    {
        if (this->mStates.empty())
			return;
        rapidjson::Value states(rapidjson::kObjectType);
		for (const auto& state : this->mStates)
		{
            rapidjson::Value key(state->GetStateId().GetFullNamespace(), allocator);
            rapidjson::Value value(rapidjson::kObjectType);
            state->WriteState(value, allocator);
            states.AddMember(key, value, allocator);
		}
		location.AddMember("states", states, allocator);
    }
}
