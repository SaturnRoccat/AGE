#include "MapColor.hpp"
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
namespace AgeData::BlockComponents
{
    inline AgeAPI::ErrorString MapColor::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const
    {
        auto& [json, alloc] = proxy;
        std::visit(
            AgeAPI::overloaded{
                [&](const MapColor::MapColorVec& color)
                {
                    rapidjson::TypeTranslation<AgeAPI::Vec3T<u8>, 3>::WriteToJson(color, json, alloc);
                },
                [&](const std::string& color)
                {
                    json.SetString(color, alloc);
                }
            },
            mValue
        );
    }
}
