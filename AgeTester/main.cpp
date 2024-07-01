#include <AgeSharedUtils.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <print>
#include <array>
#include <rapidjson/document.h>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <rapidjson/prettywriter.h>
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
#include <AgeData/BlockComponents.hpp>
	
constexpr int SIZE = 1023;
using namespace AgeAPI::Backend::Rp;
using namespace AgeAPI;


#include <random>

float RandomFloat(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}


int main(int argc, char** argv)
{
	Addon addon({ 1, 21, 0 }, { 0, 0, 1 }, "Custom Block Test", "A Test for custom blocks");
	TextureLayer core("./dirt.png", false);
	core.SetColorType(PNG_COLOR_TYPE_RGBA);
	std::unique_ptr<AddonFeatures::Block> block(new AddonFeatures::Block(
			"custom:block",
			{ 1, 21, 0 },
			"temp",
			{
				Backend::Rp::BlockResourceElement(
					"textures/block/dirt",
					std::move(core)
				)
			}
		)
	);
	using namespace AgeData::BlockComponents;
	using namespace AgeAPI::Components;
	block->AddComponentChain(
		&addon,
		PC<BlockComponentBase, CollisionBox>(
			true
		)
	).AddComponentChain(
		&addon,
		PC<BlockComponentBase, CraftingTable, CraftingTable::CraftingTableData>(
			{
				"custom table",
				{
					"crafting_table"
				}
			}
		)
	).AddComponentChain(
		&addon,
		PC<BlockComponentBase, DestructibleByExplosion>(
			200000
		)
	).AddComponentChain(
		&addon,
		PC<BlockComponentBase, Flammable, Flammable::FlammableData>(
			{
				10, 100
			}
		)
	);
	addon.AddBlock(std::move(block));
	
	addon.OutputAddon(
		{
			Addon::GetDevelopmentBehaviourPackPath(),
			Addon::GetDevelopmentResourcePackPath()
		}, "custom_block_testing");
	return 0;
}