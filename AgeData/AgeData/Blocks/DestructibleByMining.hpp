#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class DestructibleByMining : public AgeAPI::Components::BlockComponentBase
    {
    public:
        DestructibleByMining(float secondsToMine) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 16, 100 },
            "minecraft:destructible_by_mining"
        ), mValue({ secondsToMine }) {}

        DestructibleByMining(bool value = true) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 16, 100 },
            "minecraft:destructible_by_mining"
        ), mValue(value) {}

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        ToggleOrData<float, true> mValue;
    };
}