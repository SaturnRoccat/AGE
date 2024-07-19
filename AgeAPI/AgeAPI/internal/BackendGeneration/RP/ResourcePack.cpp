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

	BlockRegistrationError ResourcePack::BindBlockResource(const BlockResource& blkResource, bool sanityCheck)
	{
		if (blkResource.HasGeo())
		{
			if (blkResource.GetGeo().mGeometry.has_value())
				if (mModelManager.AddModel(blkResource.GetGeo().mGeoName, blkResource.GetGeo().mGeometry.value(), !sanityCheck) != ModelError::NONE)
					return BlockRegistrationError::MODEL_ALREADY_EXISTS;
			if (!blkResource.mSound.empty())
				// TODO: Add sound manager registeration
				if (mBlockJson.AddBlock(blkResource.mBlockName, "", blkResource.mSound, !sanityCheck) != BlockJsonError::NONE)
					return BlockJsonErrorToBlockRegistrationError(BlockJsonError::ALREADY_EXISTS);
		}
		else
		{
			if (blkResource.HoldsSingleTexture())
			{
				mBlockJson.AddBlock(
					blkResource.mBlockName,
					blkResource.mTextures.begin()->second.mTextureAlias,
					blkResource.mSound,
					!sanityCheck 
				);
			}
			else
			{
				BlockJsonStorageImpl element;
				element.mSoundID = blkResource.mSound;
				element.mTextures.resize(blkResource.mTextures.size());
				for (const auto& [side, texture] : blkResource.mTextures)
					element.mTextures.push_back({ side, texture.mTextureAlias });
				if (mBlockJson.AddBlock(blkResource.mBlockName, element, !sanityCheck) != BlockJsonError::NONE)
					return BlockRegistrationError::BLOCK_ALREADY_EXISTS;
				
			}
		}

		for (const auto& [side, texture] : blkResource.mTextures)
			if (mTerrainTextureManager.BindBlockResourceElement(texture) != TextureError::NONE)
				return BlockRegistrationError::TEXTURE_ALREADY_EXISTS;
	}
}
