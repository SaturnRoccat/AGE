#include <AgeSharedUtils.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <print>
#include <array>
#include <rapidjson/document.h>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <rapidjson/prettywriter.h>
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
#include <AgeAPI/internal/NBT/Tags.hpp>
#include <AgeAPI/internal/NBT/NBTFile.hpp>
#include <AgeData/BlockComponents.hpp>
	
using namespace AgeAPI;
using namespace AgeAPI::NBT;

int main(int argc, char** argv)
{

	NBTFile file("level.dat");
	
	i32 version = file.GetTag<TagInt>(TagString("functioncommandlimit")).GetValue();
	std::println("functioncommandlimit: {}", version);
	file[TagString("functioncommandlimit")] = TagInt(32000);
	version = file.GetTag<TagInt>(TagString("functioncommandlimit")).GetValue();
	std::println("functioncommandlimit: {}", version);






	return 0;
}