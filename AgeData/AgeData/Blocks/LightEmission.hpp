#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class LightEmission : public AgeAPI::Components::BlockComponentBase
    {
    public:
        LightEmission(int val = 15) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:light_emission"
        ), mValue(val) {
            if (val > 15)
                mValue = 15;
            else if (val < 0)
                mValue = 0;
        }

        void SetEmission(int val) {
            if (val > 15)
                mValue = 15;
            else if (val < 0)
                mValue = 0;
            else
                mValue = val;
        }
        int GetEmission() const { return mValue; }

        BlockComponentBase* Clone() const override {
            return new LightEmission(mValue);
        }

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override
        {
            proxy.mWriteLoc.SetInt64(mValue);
        }
    private:
        int mValue{};
    };
}