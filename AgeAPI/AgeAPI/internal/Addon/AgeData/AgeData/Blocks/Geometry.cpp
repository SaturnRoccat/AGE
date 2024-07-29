#include <AgeData/Blocks/Geometry.hpp>
#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
#include <ranges>

namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString Geometry::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon,
        AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const
    {
        auto& [writeLocation, alloc] = proxy;
        std::visit(AgeAPI::overloaded{
            [&](const std::string& geoIdentifier)
            {
                writeLocation.SetString(geoIdentifier, alloc);
            },
            [&](const Geometry::GeometryWithData& geoData)
            {
                writeLocation.SetObject();
                writeLocation.AddMember("identifier", geoData.mGeometry, alloc);
                rapidjson::Value geoDataValue(rapidjson::kObjectType);
                for (auto& [key, value] : geoData.mBoneVisibility)
                {
                    rapidjson::Value keyStr(key, alloc);
                    if (value.IsToggle())
                    {
                        geoDataValue.AddMember(keyStr, value.GetToggle(), alloc);
                    }
                    else
                    {
                        rapidjson::Value valueString(value.GetData(), alloc);
                        geoDataValue.AddMember(keyStr, valueString, alloc);
                    }
                }
            }
        }, mValue);
        return "";
    }
} 