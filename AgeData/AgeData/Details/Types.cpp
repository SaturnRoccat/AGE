
#include "Types.hpp"

#include <rapidjson/document.h>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeData
{

    AgeAPI::ErrorString BlockBounds::WriteToJson(const AgeAPI::JsonProxy& proxy) const
    {
        rapidjson::Value val(rapidjson::kObjectType);

        rapidjson::ValueWriteWithKey<std::array<float, 3>>::WriteToJsonValue("origin", this->first, val, proxy.mAllocator);
        rapidjson::ValueWriteWithKey<std::array<float, 3>>::WriteToJsonValue("size", this->second, val, proxy.mAllocator);
        proxy.mWriteLoc.AddMember("block_bounds", val, proxy.mAllocator);

        return "";
    }



}
