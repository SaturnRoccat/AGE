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
			const SemanticVersion& minEngineVersion = {1, 21, 0},
			const SemanticVersion& addonVersion = {0, 0, 0},
			const std::string& name = "My Addon Made With AGE",
			const std::string& description = "This add-on was generated using the AGE API",
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

		void OutputAddon(const std::string& folderName, const std::pair<std::string, std::string>& outputPath = 
			{
				GetDevelopmentBehaviourPackPath(),
				GetDevelopmentResourcePackPath()
			}, bool ClearExistingData = true, bool cacheManifest = true);
		Backend::Bp::BehaviourPack& GetBehaviourPack() { return mBp; }
		const Backend::Bp::BehaviourPack& GetBehaviourPack() const { return mBp; }
		Backend::Rp::ResourcePack& GetResourcePack() { return mRp; }
		const Backend::Rp::ResourcePack& GetResourcePack() const { return mRp; }

		static const std::string& GetDevelopmentBehaviourPackPath();
		static const std::string& GetDevelopmentResourcePackPath();
	private:

	private:
		Backend::Bp::BehaviourPack mBp{this};
		Backend::Rp::ResourcePack mRp;

		std::vector<std::unique_ptr<AddonFeatures::IBlock>> mBlocks{};

	};
}