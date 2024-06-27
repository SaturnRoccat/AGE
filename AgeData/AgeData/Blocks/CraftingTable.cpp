#include <AgeData/Blocks/CraftingTable.hpp>

#include <AgeAPI/internal/RapidJsonExtension/TypeTranslations.hpp>
namespace AgeData::BlockComponents
{
    AgeAPI::ErrorString CraftingTable::WriteToJson(AgeAPI::NonOwningPtr<AgeAPI::Addon> addon, AgeAPI::JsonProxy proxy, AgeAPI::NonOwningPtr<AgeAPI::Backend::Bp::BlockBehaviour> blk) const
    {
        auto& [value, allocator] = proxy;

        value.SetObject();
        value.AddMember("table_name", mTableName, allocator);
        if (mCraftingTags.size() > 64)
			return "CraftingTable: Crafting tags count is greater than 64";

        rapidjson::ValueWriteWithKey<std::vector<std::string>>::WriteToJsonValue("crafting_tags", mCraftingTags, value, allocator);
        return "";
    }
}