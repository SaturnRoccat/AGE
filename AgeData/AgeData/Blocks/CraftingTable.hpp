#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class CraftingTable : public AgeAPI::Components::BlockComponentBase
    {
    public:
        CraftingTable(const std::string& name, const std::vector<std::string>& craftingTags = {}) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:crafting_table"
        ), mTableName(name), mCraftingTags(craftingTags) {}

        bool IsValid() const;
        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const override;
        
        void AddCraftingTag(const std::string& tag) { mCraftingTags.push_back(tag); }
        
        const auto& GetCraftingTags() const { return mCraftingTags; }
    private:
        std::vector<std::string> mCraftingTags{};
        std::string mTableName{};
    };

}