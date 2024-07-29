#pragma once
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeData/Details/Types.hpp>

namespace AgeData::BlockComponents
{
    class CraftingTable : public AgeAPI::Components::BlockComponentBase
    {
    public:
        struct CraftingTableData
        {
            std::string name;
			std::vector<std::string> craftingTags;
            CraftingTableData(const std::string& name, const std::vector<std::string>& craftingTags) : name(name), craftingTags(craftingTags) {}
        };
    public:
        CraftingTable(const std::string& name, const std::vector<std::string>& craftingTags = {}) : AgeAPI::Components::BlockComponentBase(
            0,
            { 1, 19, 50 },
            "minecraft:crafting_table"
        ), mTableName(name), mCraftingTags(craftingTags) {}

        CraftingTable(const CraftingTableData& data) : AgeAPI::Components::BlockComponentBase(
			0,
			{ 1, 19, 50 },
			"minecraft:crafting_table"
		), mTableName(data.name), mCraftingTags(data.craftingTags) {}

        AgeAPI::ErrorString WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::AddonFeatures::Block> blk) const override;
        
        void AddCraftingTag(const std::string& tag) { mCraftingTags.push_back(tag); }

        const auto& GetCraftingTags() const { return mCraftingTags; }
        BlockComponentBase* Clone() const override 
        {
            return new CraftingTable(*this);
        }
    private:
        std::vector<std::string> mCraftingTags{};
        std::string mTableName{};
    };

}