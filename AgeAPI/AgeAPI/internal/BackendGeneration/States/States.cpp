#include "States.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
namespace AgeAPI::Backend
{
	void BoolState::WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
	{
		rapidjson::ValueWriteWithKey<std::array<bool, 2>>::WriteToJsonValue(mStateId.GetFullNamespace(), {true, false}, value, allocator);
	}
	void IntState::WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
	{
		rapidjson::ValueWriteWithKey<std::vector<int>>::WriteToJsonValue(mStateId.GetFullNamespace(), mValues, value, allocator);
	}
	void StringState::WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
	{
		rapidjson::ValueWriteWithKey<std::vector<std::string>>::WriteToJsonValue(mStateId.GetFullNamespace(), mValues, value, allocator);
	}
}
