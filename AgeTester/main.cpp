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
int main(int argc, char** argv)
{
	TextureLayer layer(AgeAPI::IVec2{ SIZE + 1, SIZE + 1}, 8);
	TextureLayer layer2(AgeAPI::IVec2{ SIZE + 1, SIZE + 1}, 8);
	TextureLayer layer3(AgeAPI::IVec2{ SIZE + 1, SIZE + 1}, 8);

	Color Col{ 1.f, 0.3f, 1.f, 1.f };
	Color Col2{ 0.3f, 1.f, 0.3f, 1.f };
	Color Col3{ 0.3f, 0.3f, 1.f, 1.f };
	Color Col4{ 1.f, 1.f, 0.3f, 1.f };

	layer.DrawRectangleOutline({ 399, 399 }, { 400, 400 }, Col2, 40);
	layer2.DrawCircleOutline({ 600, 500 }, 300, Col4, 5);
	layer3.DrawTriangle({ 0, 0 }, { 100, 100 }, { 200, 0 }, Col3);
	layer2.DrawCircle({ 150, 150 }, 100, Col);
	layer3.DrawRectangle({50, 50}, {100, 100}, Col2);


	layer.Write("test.png");
	layer2.Write("test2.png");
	layer3.Write("test3.png");

	Texture texture(std::move(layer));
	texture.AddLayer(std::move(layer2));
	texture.AddLayer(std::move(layer3));
	texture.SelectLayer(2);
	texture.SetSelectedLayerAlpha(0.5f);

	texture.FinalizeAndWrite("final.png");


	return 0;
}