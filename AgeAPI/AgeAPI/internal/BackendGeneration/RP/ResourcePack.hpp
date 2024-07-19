#pragma once
#include <AgeAPI/internal/Types.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/TerrainTextures.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/BlockJson.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ModelManager.hpp>



namespace AgeAPI::Backend::Rp
{
	enum class BlockRegistrationError
	{
		NONE,
		MODEL_ALREADY_EXISTS,
		MODEL_DOES_NOT_EXIST,
		TEXTURE_DOES_NOT_EXIST,
		TEXTURE_ALREADY_EXISTS,
		BLOCK_ALREADY_EXISTS,
		BLOCK_DOES_NOT_EXIST

	};

	class ResourcePack
	{
	private:
		Manifest mManifest{};
		TerrainTexture mTerrainTextureManager{};
		BlockJson mBlockJson{};
		ModelManager mModelManager{};
		friend class Addon;
	private:
		ResourcePack(const std::string& resource_pack_name, const std::string& atlasName = "atlas.terrain") : mTerrainTextureManager(atlasName, resource_pack_name) {}
		
		ResourcePack(const ResourcePack& other) = delete;
		ResourcePack(ResourcePack&& other) noexcept = default;

		void setManifest(Manifest& manifest) { mManifest = std::move(manifest); }
		void buildResourcePack(const std::filesystem::path& outputDir, bool Manifest) const;
		void writeTerrainTextures(const std::filesystem::path& outputBase) const;
		void writeBlockJson(const std::filesystem::path& outputBase) const;
	public:
		const Manifest& GetManifest() const { return mManifest; }
		Manifest& GetManifest() { return mManifest; }

		BlockRegistrationError BindBlockResource(const BlockResource& blkResource, bool sanityCheck = true);

	};
}