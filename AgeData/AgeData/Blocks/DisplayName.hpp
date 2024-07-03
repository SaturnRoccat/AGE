#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class DisplayName : public AgeAPI::Components::BlockComponentBase
    {
    public:
        DisplayName(const std::string& tanslationKey) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 60 },
            "minecraft:display_name"
        ), mLocalisationKey( tanslationKey ) {}


        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
    private:
        std::string mLocalisationKey{};
    };
}