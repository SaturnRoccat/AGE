#include "BehaviourPack.hpp"
#include <filesystem>
#include <fstream>

namespace AgeAPI::Backend::Bp
{
	void BehaviourPack::writeBlocks(const std::filesystem::path& outputBase)
	{
		std::filesystem::path BlocksPath = outputBase / "blocks";
		std::filesystem::create_directories(BlocksPath);
		for (i32 i = 0; i < mBlockBehaviours.size(); i++)
		{
			BlockBehaviour& blockBehaviour = mBlockBehaviours[i];
			auto BlockDocumentExpected = blockBehaviour.BuildBlockBehaviourDocument(mAddon);
			if (!BlockDocumentExpected.has_value())
				throw std::runtime_error(BlockDocumentExpected.error().GetAsString());
			rapidjson::Document& BlockDocument = BlockDocumentExpected.value();
			std::filesystem::path BlockPath = BlocksPath / std::format("block{}.json", i);
			std::ofstream BlockFile(BlockPath);
			if (!BlockFile.is_open())
				throw std::runtime_error("Failed to open block file for writing");
			rapidjson::OStreamWrapper BlockStream(BlockFile);
			rapidjson::Writer<rapidjson::OStreamWrapper> BlockWriter(BlockStream);
			BlockDocument.Accept(BlockWriter);
			BlockStream.Flush();
			BlockFile.close();
		}
	}
	void BehaviourPack::buildBehaviourPack(const std::filesystem::path& outputDir, bool Manifest)
	{
		std::filesystem::create_directories(outputDir);

		writeBlocks(outputDir);
		if (!Manifest)
			return;
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
}
