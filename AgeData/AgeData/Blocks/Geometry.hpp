#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>
#include <variant>

namespace AgeData::BlockComponents
{
    // TODO: Switch to a custom geometry string that has validation
    class Geometry : public AgeAPI::Components::BlockComponentBase
    {
    public:
        struct GeometryWithData
        {
            std::string mGeometry{};
            std::vector<
                std::pair<
                    std::string,
                    ToggleOrData<std::string, true>
            >> mBoneVisibility{};

        };
    public:
        Geometry(const std::string& geometry = "minecraft:pig") : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:geometry"
        ), mValue({geometry}) {}
        Geometry(const GeometryWithData& geometry) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:geometry"
        ), mValue({geometry}) {}



        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        std::variant<std::string, GeometryWithData> mValue{};
    };
}