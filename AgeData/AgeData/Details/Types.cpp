
#include "Types.hpp"

#include <rapidjson/document.h>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeData
{

    void BlockBounds::WriteToJson(const AgeAPI::JsonProxy& proxy)
    {
        auto& [value, allocator] = proxy;
        value.SetObject(); 
        rapidjson::ValueWriteWithKey<std::array<float, 3>>::WriteToJsonValue(
            "origin",
            this->first.ToArray(),
            value,
            allocator
        );
        rapidjson::ValueWriteWithKey<std::array<float, 3>>::WriteToJsonValue(
            "size",
            this->second.ToArray(),
            value,
            allocator);
    }



}
