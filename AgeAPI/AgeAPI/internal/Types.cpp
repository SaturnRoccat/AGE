#include "Types.hpp"
namespace AgeAPI
{
	void MenuCategory::WriteToJson(const JsonProxy proxy)
	{
		rapidjson::Value val(rapidjson::kObjectType);
		val.AddMember("category", mCategory, proxy.mAllocator);
		auto stringGroup = GetItemGroupString(mItemGroup);
		if (!stringGroup.empty())
			val.AddMember("group", std::string(stringGroup), proxy.mAllocator);
		if (!mIsHiddenInCommands)
			val.AddMember("is_hidden_in_commands", mIsHiddenInCommands, proxy.mAllocator);
		proxy.mWriteLoc.AddMember("menu_category", val, proxy.mAllocator);
	}

    template <typename T>
    std::string Vec3T<T>::WriteToJson(const JsonProxy& proxy) const
    {
        if (proxy.mWriteLoc.IsArray())
        {
            auto arr = proxy.mWriteLoc.GetArray();
            arr.PushBack(x, proxy.mAllocator);
            arr.PushBack(y, proxy.mAllocator);
            arr.PushBack(z, proxy.mAllocator);
        }
        else if (proxy.mWriteLoc.IsObject())
        {
            proxy.mWriteLoc.AddMember("x", x, proxy.mAllocator);
            proxy.mWriteLoc.AddMember("y", y, proxy.mAllocator);
            proxy.mWriteLoc.AddMember("z", z, proxy.mAllocator);
        }
        else
        {
            return "Invalid JSON Proxy";
        }
        return "";
    }
    // TODO: Convert to ErrorString
	template <typename T>
    std::string Vec4T<T>::WriteToJson(const AgeAPI::JsonProxy& proxy) const
    {
        if  (proxy.mWriteLoc.IsArray())
        {
            auto arr	= proxy.mWriteLoc.GetArray();
            arr.PushBack(x, proxy.mAllocator);
            arr.PushBack(y, proxy.mAllocator);
            arr.PushBack(z, proxy.mAllocator);
            arr.PushBack(w, proxy.mAllocator);
        }
        else if (proxy.mWriteLoc.IsObject())
        {
            proxy.mWriteLoc.AddMember("x", x, proxy.mAllocator);
            proxy.mWriteLoc.AddMember("y", y, proxy.mAllocator);
            proxy.mWriteLoc.AddMember("z", z, proxy.mAllocator);
            proxy.mWriteLoc.AddMember("w", w, proxy.mAllocator);
        } else
        {
            return "Invalid JSON Proxy";
        }
        return "";
		    
    }

}
