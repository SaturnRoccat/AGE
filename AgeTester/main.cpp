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
	Block block = Block::MakeBlock("custom:block").value();
	block
		.AddComponent(MapColor{ MapColor::MapColorVec{50, 50, 50} })
		.value()
		.AddComponent(LightEmission{ 15 })
		.value()
		.SetTexture(Texture{ {16, 16}, 16, 6, {1.f, 1.f, 0.5f, 1.f} }, std::string("custom_texture"));
	






	return 0;
}

