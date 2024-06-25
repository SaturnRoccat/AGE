#include "ResourcePack.hpp"
#include <fstream>


namespace AgeAPI::Backend::Rp
{
	void ResourcePack::buildResourcePack(const std::filesystem::path& outputDir, bool Manifest) const
	{
		std::filesystem::create_directories(outputDir);

		writeTerrainTextures(outputDir);
		writeBlockJson(outputDir);
		if (!Manifest)
			return;

		// Write manifest
		auto ManifestDocumentExpected = mManifest.WriteToDocument();
		if (!ManifestDocumentExpected.has_value())
			throw std::runtime_error(ManifestDocumentExpected.error().GetAsString());
		rapidjson::Document& ManifestDocument = ManifestDocumentExpected.value();
		std::filesystem::path ManifestPath = outputDir / "manifest.json";
		std::ofstream ManifestFile(ManifestPath);
		if (!ManifestFile.is_open())
			throw std::runtime_error("Failed to open manifest file for writing");
		rapidjson::OStreamWrapper ManifestStream(ManifestFile);
		rapidjson::Writer<rapidjson::OStreamWrapper> ManifestWriter(ManifestStream);
		ManifestDocument.Accept(ManifestWriter);
		ManifestStream.Flush();
		ManifestFile.close();


	}
	void ResourcePack::writeTerrainTextures(const std::filesystem::path& outputBase) const
	{
		std::filesystem::path TexturesPath = outputBase / "textures";
		std::filesystem::create_directories(TexturesPath);

		auto TerrainTexturesDocumentExpected = mTerrainTextureManager.WriteToDocument();
		if (!TerrainTexturesDocumentExpected.has_value())
			throw std::runtime_error(TerrainTexturesDocumentExpected.error().GetAsString());
		rapidjson::Document& TerrainTexturesDocument = TerrainTexturesDocumentExpected.value();
		std::filesystem::path TerrainTexturesPath = TexturesPath / "terrain_texture.json";
		std::ofstream TerrainTexturesFile(TerrainTexturesPath);
		if (!TerrainTexturesFile.is_open())
			throw std::runtime_error("Failed to open terrain texture file for writing");
		rapidjson::OStreamWrapper TerrainTexturesStream(TerrainTexturesFile);
		rapidjson::Writer<rapidjson::OStreamWrapper> TerrainTexturesWriter(TerrainTexturesStream);
		TerrainTexturesDocument.Accept(TerrainTexturesWriter);
		TerrainTexturesStream.Flush();
		TerrainTexturesFile.close();
		mTerrainTextureManager.Write(outputBase);


	}
	void ResourcePack::writeBlockJson(const std::filesystem::path& outputBase) const
	{
		std::filesystem::path BlockJsonPath = outputBase / "blocks.json";
		std::ofstream BlockJsonFile(BlockJsonPath);
		if (!BlockJsonFile.is_open())
			throw std::runtime_error("Failed to open block json file for writing");
		rapidjson::OStreamWrapper BlockJsonStream(BlockJsonFile);
		rapidjson::Writer<rapidjson::OStreamWrapper> BlockJsonWriter(BlockJsonStream);
		auto BlockJsonDocumentExpected = mBlockJson.BuildBlockJsonDocument();
		if (!BlockJsonDocumentExpected.has_value())
			throw std::runtime_error(BlockJsonDocumentExpected.error().GetAsString());
		BlockJsonDocumentExpected.value().Accept(BlockJsonWriter);
		BlockJsonStream.Flush();
		BlockJsonFile.close();

	}
}
