#include <AgeAPI/internal/Structures/MCStructure.hpp>
#include <AgeAPI/internal/Addon/Addon.hpp>
	
using namespace AgeAPI;
using namespace AgeAPI::NBT;
using namespace AgeAPI::Backend::Rp;

int main(int argc, char** argv)
{
	Texture texture(I16Vec2{ 16, 16 });

	BlockResourceElement block{std::move(texture), "custom:texture" };
	Identifier id("custom:block");
	Geometry geo("e", "a", "geometry.goober");
	BlockResource resource(
		{ {TextureSide::BOTTOM, block}, {TextureSide::ALL, block.ReasignAlias("custom:texture_main")} }, id, BlockResource::Geo{ geo.GetGeoName(), geo });
	Addon addon;
	auto& rp = addon.GetResourcePack();
	if (rp.BindBlockResource(resource) != BlockRegistrationError::NONE)
		return 1;




	return 0;
}