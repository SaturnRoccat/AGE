#include <AgeAPI/internal/Addon/Addon.hpp>
#include <absl/log/initialize.h>
#include <absl/log/log_sink.h>
#include <absl/log/log_sink_registry.h>
#include <absl/log/globals.h>
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
	)
	{

	}
	void Addon::OutputAddon(const std::string& folderName, const std::pair<std::string, std::string>& outputPath, bool ClearOutputFolder, bool CacheManifest)
	{
		std::filesystem::path outputDirBeh = outputPath.first;
		std::filesystem::path outputDirRes = outputPath.second;

		

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

	}

	/*
	* Steps to get the path to the development behaviour pack:
	* 1. Fetch %localappdata%/Packages/Microsoft.MinecraftUWP_8wekyb3d8bbwe/LocalState/games/com.mojang/
	* 2. Check if the folder contains a folder named "development_behavior_packs"
	* 3. If it does, return the path to the folder
	* 4. If it doesn't, create the folder and return the path to the folder
	* 5. If the folder can't be created, return an empty string
	* 
	* Same steps for resource packs, but with "development_resource_packs" instead of "development_behavior_packs"
	*/

	const std::string& Addon::GetDevelopmentBehaviourPackPath()
	{
		static std::string path;
		if (path.empty())
		{
			std::filesystem::path localAppData = std::getenv("LOCALAPPDATA");
			std::filesystem::path minecraftPath = localAppData / "Packages" / "Microsoft.MinecraftUWP_8wekyb3d8bbwe" / "LocalState" / "games" / "com.mojang";
			std::filesystem::path devBehPath = minecraftPath / "development_behavior_packs";

			if (!std::filesystem::exists(devBehPath))
			{
				if (!std::filesystem::create_directory(devBehPath))
					return path;
			}

			path = devBehPath.string();
		}

		return path;
	}
	const std::string& Addon::GetDevelopmentResourcePackPath()
	{
		static std::string path;
		if (path.empty())
		{
			std::filesystem::path localAppData = std::getenv("LOCALAPPDATA");
			std::filesystem::path minecraftPath = localAppData / "Packages" / "Microsoft.MinecraftUWP_8wekyb3d8bbwe" / "LocalState" / "games" / "com.mojang";
			std::filesystem::path devResPath = minecraftPath / "development_resource_packs";

			if (!std::filesystem::exists(devResPath))
			{
				if (!std::filesystem::create_directory(devResPath))
					return path;
			}

			path = devResPath.string();
		}

		return path;
	}
	static Addon addon;
	// The reason we have a singleton is because most of the time only a singel addon will be used but there maybe rare cases where more are needed
	// so we allow construction of multiple but the lifetime must be managed by the user
	NonOwningPtr<Addon> Addon::SetupStaticInstance(
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
		Capabilities capabilities)
	{
		addon = std::move(
			
			Addon(minEngineVersion,
				addonVersion,
				name,
				description,
				AutoRegisterBehAndResAsDeps,
				experimentalSettings,
				basePath, extraModules, dependencies,
				metadata, capabilities
			)
		);
		absl::InitializeLog();
		absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
		return &addon;
	}

	NonOwningPtr<Addon> Addon::GetStaticInstance()
	{
		return &addon;
	}
}
