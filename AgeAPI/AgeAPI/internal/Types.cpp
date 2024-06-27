#include "Types.hpp"
namespace AgeAPI
{
	void MenuCategory::WriteToJson(JsonProxy proxy)
	{
		rapidjson::Value val(rapidjson::kObjectType);
		val.AddMember("category", mCategory, proxy.mAllocator);
		auto stringGroup = GetItemGroupString(mItemGroup);
		if (!stringGroup.empty())
			val.AddMember("group", stringGroup, proxy.mAllocator);
		if (!mIsHiddenInCommands)
			val.AddMember("is_hidden_in_commands", mIsHiddenInCommands, proxy.mAllocator);
		proxy.mWriteLoc.AddMember("menu_category", val, proxy.mAllocator);
	}
}
