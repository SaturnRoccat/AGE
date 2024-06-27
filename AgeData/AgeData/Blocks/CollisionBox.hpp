#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>


namespace AgeData::BlockComponents
{
    class CollisionBox : public AgeAPI::Components::BlockComponentBase
    {
    public:
        CollisionBox(const AgeAPI::FVec3& origin, const AgeAPI::FVec3& size) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:collision_box"
        ), mValue({ origin, size }) {}

        CollisionBox(bool value) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:collision_box"
        ), mValue(value) {}

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        ToggleOrData<BlockBounds, true> mValue;
    };

    
}
