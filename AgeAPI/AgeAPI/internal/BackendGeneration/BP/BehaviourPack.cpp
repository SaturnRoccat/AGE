#include "BehaviourPack.hpp"
#include <filesystem>
#include <fstream>

namespace AgeAPI::Backend::Bp
{
	void BehaviourPack::writeBlocks(const std::filesystem::path& outputBase)
	{
		std::filesystem::path blocksPath = outputBase;
		blocksPath /= "blocks";
		forcePath(blocksPath);
		for (auto& block : mBlocks)
		{
		}

	}
	void BehaviourPack::buildBehaviourPack(const std::filesystem::path& outputDir, bool Manifest)
	{
		if (!std::filesystem::exists(outputDir))
		{
			if (!std::filesystem::create_directory(outputDir))
				throw std::runtime_error("Failed to create directory: " + outputDir.string());
		}
		writeBlocks(outputDir);
				
	}
	void BehaviourPack::forcePath(const std::filesystem::path& outputDir)
	{
		if (!std::filesystem::exists(outputDir))
		{
			if (!std::filesystem::create_directory(outputDir))
				throw std::runtime_error("Failed to create directory: " + outputDir.string());
		}
	}
}
