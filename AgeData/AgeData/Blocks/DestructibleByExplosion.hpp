#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class DestructibleByExplosion : public AgeAPI::Components::BlockComponentBase
    {
    public:
        DestructibleByExplosion(int explosionResistance) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 16, 100 },
            "minecraft:destructible_by_explosion"
        ), mValue({ explosionResistance }) {}

        DestructibleByExplosion(bool value = true) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 16, 100 },
            "minecraft:destructible_by_explosion"
        ), mValue(value) {}

        void SetDestructibleByExplosion(int explosionResistance) { mValue = explosionResistance; }
        void SetDestructibleByExplosion(bool value) { mValue = value; }
        int GetDestructibleByExplosion() const { return mValue; }
        bool GetDestructibleByExplosionBool() const { return mValue; }
        bool IsData() const { return std::holds_alternative<int>(mValue); }

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        ToggleOrData<int, true> mValue;
    };
}