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
        auto err = WriteComponents(addon, minecraftBlock, allocator);
        if (err.ContainsError())
            return err;
        err = WritePermutations(addon, minecraftBlock, allocator);
        if (err.ContainsError())
			return err;
        description.AddMember("identifier", this->mBlockIdentifier.GetFullNamespace(), allocator);
        WriteStates(description, allocator);
        mCategory.WriteToJson({description, allocator});
        minecraftBlock.AddMember("description", description, allocator);


        location.AddMember("minecraft:block", minecraftBlock, allocator);
        return ErrorString();
    }

    ErrorString BlockBehaviour::WriteComponents(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value components(rapidjson::kObjectType);
        for (const auto& [keyRawString, component] : this->mBlockComponents)
		{
            rapidjson::Value value(rapidjson::kObjectType);
            JsonProxy proxy(value, allocator);
            auto err = component->WriteToJson(addon, proxy, this);
            if (err.ContainsError() == true)
				return err;
            if (!component->IsTransient())
            {
                rapidjson::Value key(keyRawString, allocator);
                components.AddMember(key, value, allocator);
            }
		}
		location.AddMember("components", components, allocator);
        return ErrorString();
    }
    ErrorString BlockBehaviour::WritePermutations(NonOwningPtr<Addon> addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator) 
    {
        if (this->mPermutations.empty())
			return ErrorString();
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
        return ErrorString();
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
