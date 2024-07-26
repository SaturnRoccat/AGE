#include <AgeAPI/internal/Structures/MCStructure.hpp>
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <AgeData/BlockComponents.hpp>

	
using namespace AgeAPI;
using namespace AgeAPI::NBT;
using namespace AgeAPI::Backend::Rp;
using namespace AgeAPI::AddonFeatures;
using namespace AgeData::BlockComponents;

int main(int argc, char** argv)
{
	Identifier identifier = "custom:base_block";
	Block baseBlock = Block::MakeBlock("custom:base_block")
		.value()
		.SetTexture(Texture{ I16Vec2{16, 16} }, std::string("a"))
		.AddComponent(LightEmission{ 15 })
		.value()
		.AddComponent(MapColor{ MapColor::MapColorVec{50, 50, 50} })
		.value();
	Addon::SetupStaticInstance();
	auto addon = Addon::GetStaticInstance();
	addon->AddBlock(std::move(baseBlock));
	return 0;
}

