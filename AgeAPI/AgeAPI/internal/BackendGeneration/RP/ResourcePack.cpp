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


	}
	void ResourcePack::writeBlockJson(const std::filesystem::path& outputBase) const
	{

	}

	void ResourcePack::BindBlockResource(const BlockResource& blkResource)
	{
		if (blkResource.HasGeo())
		{
			if (blkResource.GetGeo().mGeometry.has_value())
				mModelManager.AddModel(blkResource.GetGeo().mGeoName, blkResource.GetGeo().mGeometry.value());
		}

		if (blkResource.HoldsSingleTexture())
		{
			auto& texture = std::get<BlockResourceElement>(blkResource.mTextures);
			mTerrainTextureManager.BindBlockResourceElement(texture);
		}
		else
		{
			auto& textureStore = std::get<std::vector<std::pair<TextureSide, BlockResourceElement>>>(blkResource.mTextures);
			for (auto& texture : textureStore)
				mTerrainTextureManager.BindBlockResourceElement(texture.second);
		}

		// Manage Case For BlockJson
		if (!blkResource.HasGeo())
		{

		}


	}
}
