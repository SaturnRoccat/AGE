#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class DisplayName : public AgeAPI::Components::BlockComponentBase
    {
    public:
        //TODO: Add an option to bind to a translation key in the lang manager @jrcarl624

        DisplayName(const std::string& tanslationKey) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 60 },
            "minecraft:display_name"
        ), mLocalisationKey( tanslationKey ) {}

        void SetLocalisationKey(const std::string& key) { mLocalisationKey = key; }
        const std::string& GetLocalisationKey() const { return mLocalisationKey; }
        BlockComponentBase* Clone() const override {
            return new DisplayName(*this);
        }

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override;
    private:
        std::string mLocalisationKey{};
    };
}