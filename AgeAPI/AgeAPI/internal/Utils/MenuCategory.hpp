#pragma once
#include <AgeAPI/internal/Utils/BasicTypeDecls.hpp>
#include <AgeAPI/internal/Utils/SmallUtilities.hpp>
#include <AgeAPI/internal/Utils/JsonProxy.hpp>
#include <AgeAPI/internal/Utils/ItemGroup.hpp>
#include <string>

namespace AgeAPI
{
	class MenuCategory
	{
	private:
		bool mIsHiddenInCommands = false;
		ItemGroup mItemGroup = ItemGroup::itemGroup_name_none;
		std::string mCategory = "construction";
	public:
		MenuCategory() = default;
		MenuCategory(const std::string& category, ItemGroup itemGroup = ItemGroup::itemGroup_name_none, bool isHiddenInCommands = false) : mItemGroup(itemGroup), mCategory(category), mIsHiddenInCommands(isHiddenInCommands) {}
		void SetItemGroup(ItemGroup group) { mItemGroup = group; }
		void SetCategory(const std::string& category) { mCategory = category; }
		void SetHiddenInCommands(bool hidden) { mIsHiddenInCommands = hidden; }
		const auto& GetItemGroup() const { return mItemGroup; }
		const auto& GetCategory() const { return mCategory; }
		const auto& IsHiddenInCommands() const { return mIsHiddenInCommands; }
		void WriteToJson(JsonProxy proxy);
	};
}