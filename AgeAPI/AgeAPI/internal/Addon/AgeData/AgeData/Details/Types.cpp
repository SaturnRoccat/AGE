
#include <AgeData/Details/Types.hpp>

#include <rapidjson/document.h>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>

namespace AgeData
{
    void BlockBounds::WriteToJson(AgeAPI::JsonProxy proxy) const
    {
        // The const casts are legal. The functions are constant but cant add const specifier to a static function
        auto& [value, allocator] = proxy;
        value.SetObject(); 
        rapidjson::ValueWriteWithKey<std::array<int, 3>>::WriteToJsonValue(
            "origin",
            AgeAPI::NoConst(this)->second.ToArray(),
            value,
            allocator
        );
        rapidjson::ValueWriteWithKey<std::array<int, 3>>::WriteToJsonValue(
            "size",
            AgeAPI::NoConst(this)->first.ToArray(),
            value,
            allocator);
    }



}
