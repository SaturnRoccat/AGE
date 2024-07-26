#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class Friction : public AgeAPI::Components::BlockComponentBase
    {
    public:
        Friction(float friction = 0.4) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:friction"
        ), mValue(friction) {}

        void SetFriction(float friction) {
            if (friction > 1.0)
                mValue = 1.0;
            else if (friction < 0.0)
                mValue = 0.0;
            else
                mValue = friction;
        }
        float GetFriction() const { return mValue; }
        BlockComponentBase* Clone() const override {
            return new Friction(mValue);
        }


        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override
        {
            proxy.mWriteLoc.SetFloat(mValue);
            return "";
        }
    private:
        float mValue{};
    };
}