
#include "Types.hpp"

#include <rapidjson/document.h>

namespace AgeData
{

    AgeAPI::ErrorString BlockBounds::WriteToJson(const AgeAPI::JsonProxy& proxy) const
    {
        rapidjson::Value val(rapidjson::kObjectType);
        rapidjson::Value origin(rapidjson::kArrayType);
        rapidjson::Value size(rapidjson::kArrayType);

        val.AddMember("origin", this->first.WriteToJson(proxy.Derive(origin)), proxy.mAllocator);
        val.AddMember("size", this->second.WriteToJson(proxy.Derive(size)), proxy.mAllocator);
        proxy.mWriteLoc.AddMember("block_bounds", val, proxy.mAllocator);

        return "";
    }



}
