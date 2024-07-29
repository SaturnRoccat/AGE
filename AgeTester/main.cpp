#include <AgeAPI/internal/Structures/MCStructure.hpp>
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <AgeData/BlockComponents.hpp>

	
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
	Block baseBlock = Block::MakeBlock("custom:base_block")
		.value()
		.SetTexture(Texture{ I16Vec2{16, 16} }, std::string("a"))
		.AddComponents(
			MakeComponentStore(
				LightEmission{ 15 },
				MapColor{ MapColor::MapColorVec{50, 50, 50} }
			))
		.value()
		.AddPermutation(
			Permutation("Condition")
			.AddComponents(
				MakeComponentStore(
					LightEmission{ 10 },
					MapColor{ MapColor::MapColorVec{100, 100, 100} }
				)
			)
			.value()
		)
		.value()
		.AddStates(
			MakeStateStore(
				BoolState("state:is_cool"),
				IntState("state:coolness", 0, 10),
				StringState("state:owner_names", { "custom", "goober", "extra_goober" })
			)
		)
		.value();
	addon->AddBlock(std::move(baseBlock));
	return 0;
}

