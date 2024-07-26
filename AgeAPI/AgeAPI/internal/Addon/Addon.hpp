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


		void OutputAddon(const std::string& folderName, const std::pair<std::string, std::string>& outputPath = 
			{
				GetDevelopmentBehaviourPackPath(),
				GetDevelopmentResourcePackPath()
			}, bool ClearExistingData = true, bool cacheManifest = true);
		

		static const std::string& GetDevelopmentBehaviourPackPath();
		static const std::string& GetDevelopmentResourcePackPath();
		static NonOwningPtr<Addon> SetupStaticInstance(
			const SemanticVersion& minEngineVersion = { 1, 21, 0 },
			const SemanticVersion& addonVersion = { 0, 0, 0 },
			const std::string& name = "My Addon Made With AGE",
			const std::string& description = "This add-on was generated using the AGE API",
			bool AutoRegisterBehAndResAsDeps = true,
			ExperimentalSettings experimentalSettings = { 0 },
			const std::string& basePath = GetCurrentWorkingDirectory(),
			const std::vector<Module>& extraModules = {},
			const std::vector<Dependency>& dependencies = {},
			const Metadata& metadata = {},
			Capabilities capabilities = {});
		static NonOwningPtr<Addon> GetStaticInstance();
	private:

	private:

		std::vector<std::unique_ptr<AddonFeatures::Block>> mBlocks{};

	};
}