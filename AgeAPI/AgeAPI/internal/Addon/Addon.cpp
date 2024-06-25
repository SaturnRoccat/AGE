#include <AgeAPI/internal/Addon/Addon.hpp>
namespace AgeAPI
{
	Addon::Addon(
		const SemanticVersion& minEngineVersion,
		const SemanticVersion& addonVersion,
		const std::string& name,
		const std::string& description,
		bool AutoRegisterBehAndResAsDeps,
		ExperimentalSettings experimentalSettings,
		const std::string& basePath,
		const std::vector<Module>& extraModules,
		const std::vector<Dependency>& dependencies,
		const Metadata& metadata,
		Capabilities capabilities
	) : mRp(name)
	{
		Manifest behaviourManifest{
			minEngineVersion,
			addonVersion,
			name,
			description,
			{Module("data")},
			dependencies,
			metadata,
			capabilities
		};

		for (const auto& module : extraModules)
			behaviourManifest.AddModule(module);

		Manifest resourceManifest{
			minEngineVersion,
			addonVersion,
			name,
			description,
			{Module("resources")},
			dependencies,
			metadata,
			capabilities
		};

		if (AutoRegisterBehAndResAsDeps)
		{
			Dependency behDep{ behaviourManifest.GetUUID(), addonVersion};
			Dependency resDep{ resourceManifest.GetUUID(), addonVersion};

			behaviourManifest.AddDependency(resDep);
			resourceManifest.AddDependency(behDep);
		}

		mBp.setManifest(behaviourManifest);
		mRp.setManifest(resourceManifest);
	}
	void Addon::OutputAddon(const std::pair<std::string, std::string>& outputPath, const std::string& folderName, bool ClearOutputFolder, bool CacheManifest)
	{
		std::filesystem::path outputDirBeh = outputPath.first;
		std::filesystem::path outputDirRes = outputPath.second;

		for (auto& block : mBlocks)
			block->WriteToPacks(mBp, mRp);

		auto bpOutput = outputDirBeh / std::format("{}Bp", folderName);
		auto rpOutput = outputDirRes / std::format("{}Rp", folderName);
		bool generateBehaviourManifest = true;
		bool generateResourceManifest = true;

		if (ClearOutputFolder)
		{
			// Clear the output folder if it exists and keep manifest.json
			if (std::filesystem::exists(bpOutput))
				for (const auto& entry : std::filesystem::directory_iterator(bpOutput))
					if (entry.path().filename() != "manifest.json")
						std::filesystem::remove_all(entry.path());
					else
						generateBehaviourManifest = false;
			if (std::filesystem::exists(rpOutput))
				for (const auto& entry : std::filesystem::directory_iterator(rpOutput))
					if (entry.path().filename() != "manifest.json")
						std::filesystem::remove_all(entry.path());
					else
						generateResourceManifest = false;

		}

		mBp.buildBehaviourPack(bpOutput, generateBehaviourManifest);
		mRp.buildResourcePack(rpOutput, generateResourceManifest);
	}
}
