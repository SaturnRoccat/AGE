#pragma once
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>
#include <tuple>


namespace AgeAPI
{
	class Addon
	{
	public:
		Addon(
			const SemanticVersion& minEngineVersion,
			const SemanticVersion& addonVersion,
			const std::string& name,
			const std::string& description,
			bool AutoRegisterBehAndResAsDeps = true,
			ExperimentalSettings experimentalSettings = {0},
			const std::string& basePath = GetCurrentWorkingDirectory(),
			const std::vector<Module>& extraModules = {},
			const std::vector<Dependency>& dependencies = {},
			const Metadata& metadata = {},
			Capabilities capabilities = {}
		);

		Manifest& GetBehaviourManifest() { return mBp.GetManifest(); }
		Manifest& GetResourceManifest() { return mRp.GetManifest(); }

		void AddBlock(std::unique_ptr<AddonFeatures::IBlock> block) { mBlocks.push_back(std::move(block)); }

		void OutputAddon(const std::pair<std::string, std::string>& outputPath, const std::string& folderName, bool ClearExistingData = true, bool cacheManifest = true);
	private:

	private:
		Backend::Bp::BehaviourPack mBp{this};
		Backend::Rp::ResourcePack mRp;

		std::vector<std::unique_ptr<AddonFeatures::IBlock>> mBlocks{};

	};
}