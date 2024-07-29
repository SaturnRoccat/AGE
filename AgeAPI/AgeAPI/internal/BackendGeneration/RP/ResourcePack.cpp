#include "ResourcePack.hpp"
#include <fstream>
#include <AgeAPI/internal/RapidJsonExtension/FileIO.hpp>

namespace AgeAPI::Backend::Rp
{
	void ResourcePack::buildResourcePack(const std::filesystem::path& outputDir, bool Manifest) const
	{
		std::filesystem::create_directories(outputDir);

		writeTerrainTextures(outputDir);
		writeBlockJson(outputDir);
		if (!Manifest)
			return;

		auto ManifestDocumentExpected = mManifest.WriteToDocument();
		if (!ManifestDocumentExpected.has_value())
			throw std::runtime_error(ManifestDocumentExpected.error().GetAsString());
		rapidjson::Document& ManifestDocument = ManifestDocumentExpected.value();
		std::filesystem::path ManifestPath = outputDir / "manifest.json";
		auto err = rapidjson::WriteJsonFile(ManifestDocument, ManifestPath);
		if (err.ContainsError())
		{
			std::println("Failed to write manifest: {}", err.GetAsString());
			throw std::runtime_error("Failed to write manifest");
		}


	}
	void ResourcePack::writeTerrainTextures(const std::filesystem::path& outputBase) const
	{
		auto err = mTerrainTextureManager.writeTextureData(outputBase);
		if (err.ContainsError())
		{
			std::println("Failed to write terrain textures: {}", err.GetAsString());
			throw std::runtime_error("Failed to write terrain textures");
		}
	}
	void ResourcePack::writeBlockJson(const std::filesystem::path& outputBase) const
	{
		auto err = mBlockJson.writeToFile(outputBase);
		if (err.ContainsError())
		{
			std::println("Failed to write block json: {}", err.GetAsString());
			throw std::runtime_error("Failed to write block json");
		}

	}

	static BlockRegistrationError constexpr BlockJsonErrorToBlockRegistrationError(BlockJsonError error)
	{
		switch (error)
		{
		case BlockJsonError::ALREADY_EXISTS:
			return BlockRegistrationError::BLOCK_ALREADY_EXISTS;
		case BlockJsonError::NOT_FOUND:
			return BlockRegistrationError::BLOCK_DOES_NOT_EXIST;
		case BlockJsonError::NONE:
			return BlockRegistrationError::NONE;
		}
	}

	static BlockRegistrationError constexpr TextureErrorToBlockRegistrationError(TextureError error)
	{
		switch (error)
		{
		case TextureError::TEXTURE_ALREADY_EXISTS:
			return BlockRegistrationError::TEXTURE_ALREADY_EXISTS;
		case TextureError::TEXTURE_DOES_NOT_EXIST:
			return BlockRegistrationError::TEXTURE_DOES_NOT_EXIST;
		case TextureError::NONE:
			return BlockRegistrationError::NONE;
		}
	}

		
}
