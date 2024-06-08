#include "States.hpp"

namespace AgeAPI::Backend
{
	void BoolState::WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
	{
		rapidjson::Value values(rapidjson::kArrayType);
		values.PushBack(false, allocator);
		values.PushBack(true, allocator);
		auto keyStr = mStateId.GetFullNamespace();
		rapidjson::Value key(keyStr, allocator);
		value.AddMember(key, values, allocator);
	}
	void IntState::WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
	{
		rapidjson::Value values(rapidjson::kArrayType);
		std::ranges::for_each(mValues, [&values, &allocator](int val) {
			values.PushBack(val, allocator);
		});
		auto keyStr = mStateId.GetFullNamespace();
		rapidjson::Value key(keyStr, allocator);
		value.AddMember(key, values, allocator);
	}
	void StringState::WriteState(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
	{
		rapidjson::Value values(rapidjson::kArrayType);
		std::ranges::for_each(mValues, [&values, &allocator](const std::string& str) {
			rapidjson::Value strValue(str.c_str(), allocator);
			values.PushBack(strValue, allocator);
		});
		auto keyStr = mStateId.GetFullNamespace();
		rapidjson::Value key(keyStr, allocator);
		value.AddMember(key, values, allocator);
	}
}
