#include <AgeSharedUtils.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <print>
#include <array>
#include <rapidjson/document.h>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <rapidjson/prettywriter.h>
#include <AgeAPI/internal/Addon/Addon.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/Textures/Texture.hpp>
	
constexpr int SIZE = 1023;
using namespace AgeAPI::Backend::Rp;
using namespace AgeAPI;

class MaterialInstance : public Components::BlockComponentBase
{
private:
	std::string mMaterialName;
public:
	MaterialInstance(const std::string& MaterialName) : BlockComponentBase(0, {1, 19,40}, "minecraft:material_instances"), mMaterialName(MaterialName) {}

	virtual ErrorString WriteToJson(NonOwningPtr<Addon> addon, JsonProxy proxy, NonOwningPtr<Backend::Bp::BlockBehaviour> blk) const override
	{
		rapidjson::Value val(rapidjson::kObjectType);
		val.AddMember("texture", mMaterialName, proxy.mAllocator);
		proxy.mWriteLoc.AddMember("*", val, proxy.mAllocator);
		return "";
	}

};

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
	TextureLayer core("./leaves_oak_opaque.png", false);
	core.SetColorType(PNG_COLOR_TYPE_RGBA);
	Color col { 1.f, 1.f, 1.f, 1.f };
	Color AdditionColor {0.01f, 0.01f, 0.01f, 1.f};
	Texture textureStore(std::move(core));
	for (uint64_t i = 0; i < 256; i++)
	{
		auto text = textureStore;
		text.GetSelectedLayer().TL.Multiply(col);
		AddonFeatures::Block block(
			std::format("custom:block_{}", i),
			{ 1, 21, 0 },
			std::format("custom_texture_{}", i),
			{
				BlockResourceElement(
					std::format("textures/blocks/custom_block_{}", i),
					text
				)
			}
		);
		block.SetCategory("nature");
		auto blkPtr = std::make_unique<AddonFeatures::Block>(std::move(block));
		addon.AddBlock(std::move(blkPtr));

		col = { RandomFloat(0.1f, 1.f), RandomFloat(0.1f, 1.f), RandomFloat(0.1f, 1.f), 1.f };
		//AdditionColor = { RandomFloat(0.3f, 1.f), RandomFloat(0.3f, 1.f), RandomFloat(0.3f, 1.f), 1.f };
	}
	addon.OutputAddon(
		{
			"C:\\Users\\Duckos\\AppData\\Roaming\\.minecraft_bedrock\\installations\\Duckos\\Latest Release\\packageData\\development_behavior_packs\\",
			"C:\\Users\\Duckos\\AppData\\Roaming\\.minecraft_bedrock\\installations\\Duckos\\Latest Release\\packageData\\development_resource_packs\\"
		}, "custom_block_testing");
	return 0;
}