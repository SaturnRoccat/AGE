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



        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override
        {
            proxy.mWriteLoc.SetFloat(mValue);
        }
    private:
        float mValue{};
    };
}