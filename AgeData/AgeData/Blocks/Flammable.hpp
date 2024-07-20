#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class Flammable : public AgeAPI::Components::BlockComponentBase
    {
    public:
        struct FlammableData
        {
            int mCatchChanceModifier{};
            int mDestroyChanceModifier{};
        };
    public:
        Flammable(FlammableData flamability) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 16, 100 },
            "minecraft:flammable"
        ), mValue({ flamability }) {}

        Flammable(bool value = false) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 16, 100 },
            "minecraft:flammable"
        ), mValue(value) {}

        void SetFlamability(FlammableData flamability) { mValue = flamability; }
        void SetFlamability(bool value) { mValue = value; }
        FlammableData GetFlamability() const { return mValue.GetData(); }
        bool GetFlamabilityBool() const { return mValue.GetToggle(); }
        bool IsData() const { return !mValue.IsToggle(); }


        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        ToggleOrData<FlammableData, false> mValue;
    };
}