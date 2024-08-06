#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    using AgeAPI::u8;
    using AgeAPI::IVec3;
    using AgeAPI::FVec3;
    class Rotatable : public AgeAPI::Components::BlockComponentBase
    {
    public:
        Rotatable( bool AllowRotation = false
        ) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 60 },
            "age_api:Rotatable",
            false,
            true
        ), mAllowRotation(AllowRotation)
		 {}

        BlockComponentBase* Clone() const override {
            return new Rotatable(*this);
        }

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override 
            { return ""; }

        AgeAPI::ErrorString OnComponentAdded(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) override;
    private:
        bool mAllowRotation{ false };
    };

    class LogRotation : public AgeAPI::Components::BlockComponentBase
    {
    public:
        LogRotation() : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 60 },
            "age_api:log_rotatable",
            false,
            true
        )
        {}

        BlockComponentBase* Clone() const override {
            return new LogRotation(*this);
        }

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override
        {
            return "";
        }

        AgeAPI::ErrorString OnComponentAdded(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) override;
    private:
    };
}