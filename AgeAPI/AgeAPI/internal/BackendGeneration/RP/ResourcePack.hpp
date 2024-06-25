#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/TerrainTextures.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/BlockJson.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>


namespace AgeAPI::Backend::Rp
{
	class ResourcePack
	{
	private:
		Manifest mManifest{};
		TerrainTexture mTerrainTextureManager{};
		BlockJson mBlockJson{};
		friend class Addon;
	private:
		ResourcePack(const std::string& resource_pack_name, const std::string& atlasName = "atlas.terrain") : mTerrainTextureManager(atlasName, resource_pack_name) {}
		
		ResourcePack(const ResourcePack& other) = delete;
		ResourcePack(ResourcePack&& other) noexcept
		: mManifest(std::move(other.mManifest)), mTerrainTextureManager(std::move(other.mTerrainTextureManager)) {}

		void setManifest(Manifest& manifest) { mManifest = std::move(manifest); }
		void buildResourcePack(const std::filesystem::path& outputDir, bool Manifest) const;
		void writeTerrainTextures(const std::filesystem::path& outputBase) const;
		void writeBlockJson(const std::filesystem::path& outputBase) const;
	public:

		const Manifest& GetManifest() const { return mManifest; }
		Manifest& GetManifest() { return mManifest; }

		void AddBlockResource(BlockResource&& blockResource) { mTerrainTextureManager.AddTexture(std::move(blockResource)); }
		void AddBlockJson(const BlockJson& blockJson) { mBlockJson = blockJson; }
		void AddBlockJson(BlockJson&& blockJson) { mBlockJson = std::move(blockJson); }
		void AddNewBlockJson(const std::string& key, const BlockJsonEntry& blockJsonEntry) { mBlockJson.AddEntry(key, blockJsonEntry); }




	};
}