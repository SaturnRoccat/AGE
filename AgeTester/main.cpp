#include <AgeAPI/internal/Structures/MCStructure.hpp>
#include <AgeAPI/internal/Addon/Addon.hpp>
	
using namespace AgeAPI;
using namespace AgeAPI::NBT;

int main(int argc, char** argv)
{
	Structure::MCBlock block("minecraft:stone");
	Structure::MCBlock block2("minecraft:water");

	Structure::MCStructure structure({ 16 * 10, 128, 16 * 10}, "gregery");
	int StoneIndex = structure.RegisterBlock(block);
	int AirIndex = structure.RegisterBlock(block2);
	for (auto& block : structure)
		block = StoneIndex;
	structure.WriteToFile(Addon::GetDevelopmentBehaviourPackPath() + "/custom_block_testingBp/structures/");






	return 0;
}