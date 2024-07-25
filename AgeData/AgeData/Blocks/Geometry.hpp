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
        // TODO: Add ctor to bind to geometry file @Duckos-Mods

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

        Geometry(GeometryWithData&& geo) : AgeAPI::Components::BlockComponentBase(
			0,
			{ 1, 19, 50 },
			"minecraft:geometry"
		), mValue({std::move(geo)}) {}

        void SetGeometry(const std::string& geometry) { mValue = geometry; }
        void SetGeometry(const GeometryWithData& geometry) { mValue = geometry; }
        bool IsGeometryString() const { return std::holds_alternative<std::string>(mValue); }
        std::string GetGeometry() const { return std::get<std::string>(mValue); }
        GeometryWithData GetGeometryData() const { return std::get<GeometryWithData>(mValue); }
        BlockComponentBase* Clone() const override {
            return new Geometry(*this);
        }


        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        std::variant<std::string, GeometryWithData> mValue{};
    };
}