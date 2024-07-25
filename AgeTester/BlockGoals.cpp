
// Single block
Block blockInstance("custom:block", { 1,21,20 }, {}/*optional Catagory*/, {}/*optional group*/, false/*optional command*/)
.AddComponent(MapColor{ 50, 50, 50 })
.AddComponent(LightEmission{ 15 })
.SetTexture(Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f))
.AddTranslation("EN_US", "Super Cool Block")
addon.AddBlock(blockInstance);

// Multiple blocks with shared components

std::vector<Component> sharedComponents{
	MapColor{ 50, 50, 50 },
	LightEmission{ 15 }
};
std::vector<Block> blocks{};
Block baseBlockInstance({ 1,21,20 })
.AddComponents(sharedComponents);

for (int i = 0; i < 10; i++)
{
	Block blockInstance = baseBlockInstance;
	blockInstance.SetID("custom:block" + std::to_string(i))
	.SetTexture(Texture{ "PathToMyTexture" + std::to_string(i) }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f))
	.AddTranslation("EN_US", "Super Cool Block " + std::to_string(i))
	blocks.push_back(blockInstance);
}
addon.AddBlocks(blocks);
// Multiple blocks with shared components with no Block vector

std::vector<Component> sharedComponents{
	MapColor{ 50, 50, 50 },
	LightEmission{ 15 }
};
Block baseBlockInstance({ 1,21,20 })
.AddComponents(sharedComponents);

for (int i = 0; i < 10; i++)
{
	Block blockInstance = baseBlockInstance;
	blockInstance.SetID("custom:block" + std::to_string(i))
	.SetTexture(Texture{ "PathToMyTexture" + std::to_string(i) }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f))
	.AddTranslation("EN_US", "Super Cool Block " + std::to_string(i))
    addon.AddBlock(blocks);
}

// Single block with multi side textures in single func calls
Block blockInstance("custom:block", { 1,21,20 }, {}/*optional Catagory*/, {}/*optional group*/, false/*optional command*/)
.AddComponent(MapColor{ 50, 50, 50 })
.AddComponent(LightEmission{ 15 })
.SetTexture(Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Top)
.SetTexture(Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Bottom)
.SetTexture(Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Front)
.SetTexture(Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Back)
.SetTexture(Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Left)
.SetTexture(Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Right)
.AddTranslation("EN_US", "Super Cool Block")
addon.AddBlock(blockInstance);

// Single block with multi side textures in 1 func via vector
Block blockInstance("custom:block", { 1,21,20 }, {}/*optional Catagory*/, {}/*optional group*/, false/*optional command*/)
.AddComponent(MapColor{ 50, 50, 50 })
.AddComponent(LightEmission{ 15 })
.AddTexture({
	{ Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Top },
	{ Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Bottom },
	{ Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Front },
	{ Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Back },
	{ Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Left },
	{ Texture{ "PathToMyTexture" }.Resize(32, 32).Fill(1.f, 1.f, 0.4f, 0.6f), Side::Right }
})
.AddTranslation("EN_US", "Super Cool Block")
addon.AddBlock(blockInstance);






