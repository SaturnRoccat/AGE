#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/Components/BehaviourComponents/Block/BlockComponentBase.hpp>
#include <AgeAPI/internal/BackendGeneration/States/States.hpp>
#include <vector>
#include <unordered_map>
#include <AgeAPI/internal/BackendGeneration/Permutations/Permutation.hpp>

namespace AgeAPI::Backend::Bp
{
	// Dumped from the BDS via strings fuck you mojang for removing symbols
	enum class ItemGroup
	{
		itemGroup_name_planks,
		itemGroup_name_walls,
		itemGroup_name_fence,
		itemGroup_name_fenceGate,
		itemGroup_name_stairs,
		itemGroup_name_door,
		itemGroup_name_trapdoor,
		itemGroup_name_element,
		itemGroup_name_glass,
		itemGroup_name_glassPane,
		itemGroup_name_slab,
		itemGroup_name_stoneBrick,
		itemGroup_name_sandstone,
		itemGroup_name_copper,
		itemGroup_name_wool,
		itemGroup_name_woolCarpet,
		itemGroup_name_concretePowder,
		itemGroup_name_concrete,
		itemGroup_name_stainedClay,
		itemGroup_name_glazedTerracotta,
		itemGroup_name_ore,
		itemGroup_name_stone,
		itemGroup_name_log,
		itemGroup_name_wood,
		itemGroup_name_leaves,
		itemGroup_name_sapling,
		itemGroup_name_seed,
		itemGroup_name_crop,
		itemGroup_name_grass,
		itemGroup_name_coral_decorations,
		itemGroup_name_flower,
		itemGroup_name_dye,
		itemGroup_name_rawFood,
		itemGroup_name_mushroom,
		itemGroup_name_monsterStoneEgg,
		itemGroup_name_mobEgg,
		itemGroup_name_coral,
		itemGroup_name_sculk,
		itemGroup_name_helmet,
		itemGroup_name_chestplate,
		itemGroup_name_leggings,
		itemGroup_name_boots,
		itemGroup_name_sword,
		itemGroup_name_axe,
		itemGroup_name_pickaxe,
		itemGroup_name_shovel,
		itemGroup_name_hoe,
		itemGroup_name_arrow,
		itemGroup_name_cookedFood,
		itemGroup_name_miscFood,
		itemGroup_name_goatHorn,
		itemGroup_name_horseArmor,
		itemGroup_name_potion,
		itemGroup_name_splashPotion,
		itemGroup_name_lingeringPotion,
		itemGroup_name_bed,
		itemGroup_name_candles,
		itemGroup_name_chemistrytable,
		itemGroup_name_anvil,
		itemGroup_name_chest,
		itemGroup_name_shulkerBox,
		itemGroup_name_record,
		itemGroup_name_sign,
		itemGroup_name_hanging_sign,
		itemGroup_name_skull,
		itemGroup_name_enchantedBook,
		itemGroup_name_boat,
		itemGroup_name_chestboat,
		itemGroup_name_rail,
		itemGroup_name_minecart,
		itemGroup_name_buttons,
		itemGroup_name_pressurePlate,
		itemGroup_name_banner,
		itemGroup_name_banner_pattern,
		itemGroup_name_potterySherds,
		itemGroup_name_smithing_templates,
		itemGroup_name_firework,
		itemGroup_name_fireworkStars,
		itemGroup_name_dirt,
		itemGroup_name_cobblestone,
		itemGroup_name_natureBuildingBlocks,
		itemGroup_name_sand,
		itemGroup_name_gravel,
		itemGroup_name_oreBlocks,
		itemGroup_name_redstoneContainers,
		itemGroup_name_musicBlocks,
		itemGroup_name_pistons,
		itemGroup_name_natureMisc,
		itemGroup_name_greenery,
		itemGroup_name_brick,
		itemGroup_name_tnt,
		itemGroup_name_enchantingTable,
		itemGroup_name_constructionMisc,
		itemGroup_name_torch,
		itemGroup_name_craftingTables,
		itemGroup_name_furnaces,
		itemGroup_name_climbing,
		itemGroup_name_redstoneProducers,
		itemGroup_name_redstone,
		itemGroup_name_ice,
		itemGroup_name_cactus,
		itemGroup_name_clay,
		itemGroup_name_glowstone,
		itemGroup_name_pumpkins,
		itemGroup_name_ironFence,
		itemGroup_name_beacon,
		itemGroup_name_endPortalFrame,
		itemGroup_name_eggBlocks,
		itemGroup_name_lights,
		itemGroup_name_purpur,
		itemGroup_name_endRod,
		itemGroup_name_permission,
		itemGroup_name_camera,
		itemGroup_name_lectern,
		itemGroup_name_grindStone,
		itemGroup_name_bell,
		itemGroup_name_composter,
		itemGroup_name_chiseledBookshelf
	};
	class BlockBehaviour
	{
	public:
		BlockBehaviour() = default;
		BlockBehaviour(const BlockBehaviour&) = delete;
		BlockBehaviour(BlockBehaviour&&) = default;
		BlockBehaviour(
			const Identifier& blockIdentifier,
			const SemanticVersion& formatVersion,
			bool isHiddenInCommands = false
		) : mBlockIdentifier(blockIdentifier), mFormatVersion(formatVersion), mIsHiddenInCommands(isHiddenInCommands) {}

		ErrorString AddBlockComponent(std::unique_ptr<Addon>& addon, std::unique_ptr<Components::BlockComponentBase>& component) 
		{ 
			if (component->GetFormatVersion().GetVersion() > mFormatVersion.GetVersion())
				return ErrorString("Component version is higher than the block behaviour version");
			auto it = mBlockComponents.find(component->GetComponentID().GetFullNamespace());
			if (it != mBlockComponents.end() && !component->IsTransient())
				return ErrorString("Component already exists");
			else if (component->IsTransient() && it != mBlockComponents.end())
				return it->second->MergeTransient(addon, this, component);
			else
				mBlockComponents[component->GetComponentID().GetFullNamespace()] = std::move(component);
			return "";
		}
		void AddState(std::unique_ptr<AState> state) { mStates.emplace_back(std::move(state)); }
		void AddPermutation(Permutation&& permutation) { mPermutations.emplace_back(std::move(permutation)); }

		const auto& GetBlockComponents() const { return mBlockComponents; }
		const auto& GetStates() const { return mStates; }
		const auto& GetPermutations() const { return mPermutations; }

		ErrorString BuildBlockBehaviourJson(std::unique_ptr<Addon>& addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator);
		inline std::expected<rapidjson::Document, ErrorString> BuildBlockBehaviourDocument(std::unique_ptr<Addon>& addon)
		{
			auto doc = rapidjson::Document{};
			doc.SetObject();
			auto& allocator = doc.GetAllocator();
			auto err = BuildBlockBehaviourJson(addon, doc, allocator);
			if (err.ContainsError())
				return std::unexpected(err);
			return doc;
		}
	private:
		ErrorString WriteComponents(std::unique_ptr<Addon>& addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator);
		ErrorString WritePermutations(std::unique_ptr<Addon>& addon, rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator);
		void WriteStates(rapidjson::Value& location, rapidjson::Document::AllocatorType& allocator);
	private:
		std::unordered_map<std::string, std::unique_ptr<Components::BlockComponentBase>> mBlockComponents{};
		std::vector<std::unique_ptr<AState>> mStates{};
		std::vector<Permutation> mPermutations{};
		std::string mCategory{}; // This should be an enum but i cant remember the values so its a string for now 

		Identifier mBlockIdentifier{};
		SemanticVersion mFormatVersion{};
		bool mIsHiddenInCommands = false;

	};
}
 