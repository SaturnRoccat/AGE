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

        void SetColor(MapColorVec color) { mValue = color; }
        void SetColor(const std::string& color) {
            if (color[0] != '#') throw std::runtime_error("Invalid Color Format");
            mValue = color;
        }
        BlockComponentBase* Clone() const override {
            return new MapColor(*this);
        }

        MapColorVec GetColor() const { return std::get<MapColorVec>(mValue); }
        std::string GetColorString() const { return std::get<std::string>(mValue); }
        auto GetValue() const { return mValue; }
        bool IsColorVec() const { return std::holds_alternative<MapColorVec>(mValue); }
        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        std::variant<MapColorVec, std::string> mValue;
    };
}