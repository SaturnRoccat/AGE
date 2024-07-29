#include <AgeAPI/internal/Structures/MCStructure.hpp>
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <AgeData/BlockComponents.hpp>
#include <absl/log/check.h>
	
using namespace AgeAPI;
using namespace AgeAPI::NBT;
using namespace AgeAPI::Backend::Rp;
using namespace AgeAPI::Backend;
using namespace AgeAPI::Backend::Bp;
using namespace AgeAPI::AddonFeatures;
using namespace AgeData::BlockComponents;

int main(int argc, char** argv)
{
	Addon::SetupStaticInstance();
	auto addon = Addon::GetStaticInstance();
	Block baseBlock = Block::MakeBlock("custom:base_block").value()
		.AddPermutation(
			Permutation("Condition")
			.AddComponents(
				MakeComponentStore(
					LightEmission{ 10 },
					MapColor{ MapColor::MapColorVec{100, 100, 100} }
				)
			)
		)
		.AddComponents(
			MakeComponentStore(
				LightEmission{ 5 },
				MapColor{ MapColor::MapColorVec{50, 50, 50} }
			)
		)
		.SetTexture(Texture{ I16Vec2{16, 16} }, std::string("a"))
		.AddStates(
			MakeStateStore(
				BoolState("state:is_cool"),
				IntState("state:coolness", 0, 10),
				StringState("state:owner_names", { "custom", "goober", "extra_goober" })
			)
		)
		.AddTrait(PlacementDirectionTrait{MixEnum<EnabledStates>(EnabledStates::Facing)});
	auto ptr = baseBlock.GetPermutation("Condition");
	CHECK(ptr) << "Permutation not found";
	addon->AddBlock(std::move(baseBlock));
	return 0;
}

