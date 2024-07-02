#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    using AgeAPI::u8;
    class MapColor : public AgeAPI::Components::BlockComponentBase
    {
    public:
        using MapColorVec = AgeAPI::Vec3T<u8>;
        MapColor(MapColorVec color = {255, 255, 255}) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:map_color"
        ), mValue({ color }) {}
        MapColor(const std::string& color) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:map_color"
        ), mValue( color ) {
            if (color[0] != '#') throw std::runtime_error("Invalid Color Format");
        }

        



        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        std::variant<MapColorVec, std::string> mValue;
    };
}