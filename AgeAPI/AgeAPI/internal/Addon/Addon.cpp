#include <AgeAPI/internal/Addon/Addon.hpp>
#include <absl/log/initialize.h>
#include <absl/log/log.h>
#include <absl/log/log_sink.h>
#include <absl/log/log_sink_registry.h>
#include <AgeAPI/internal/RapidJsonExtension/FileIO.hpp>
#include <absl/log/globals.h>
namespace AgeAPI
{
	Addon::Addon(const Manifest& bpManifest, const Manifest& rpManifest, bool AutoRegisterBehAndResAsDeps) : 
		mBehaviourPackManifest(bpManifest), mResourcePackManifest(rpManifest)
	{
		mName = mBehaviourPackManifest.GetName();
		if (AutoRegisterBehAndResAsDeps)
		{
			mBehaviourPackManifest.AddDependency({ mResourcePackManifest.GetUUID(), mResourcePackManifest.GetAddonVersion() });
			mResourcePackManifest.AddDependency({ mBehaviourPackManifest.GetUUID(), mBehaviourPackManifest.GetAddonVersion() });
		}
		mBehaviourPackManifest.AddModule(Module{
			"data",
			mBehaviourPackManifest.GetDescription(),
			mBehaviourPackManifest.GetAddonVersion(),
			GetUUIDString(),
			});
		mResourcePackManifest.AddModule(Module{
			"resources",
			mResourcePackManifest.GetDescription(),
			mResourcePackManifest.GetAddonVersion(),
			GetUUIDString(),
			});


	}
	void Addon::OutputAddon(bool CacheManifest, const std::string& folderName, const std::pair<std::string, std::string>& outputPath, bool ClearOutputFolder)
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
		LOG(INFO) << "Generating Behaviour Pack";
		generateBehaviourPack(bpOutput, !generateBehaviourManifest, CacheManifest);
		LOG(INFO) << "Generating Resource Pack";
		generateResourcePack(rpOutput, !generateResourceManifest, CacheManifest);
		LOG(INFO) << "Outputted Addon: " << mName;



	}


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
	const std::string& Addon::GetPreviewDevelopmentBehaviourPackPath()
	{
		static std::string path;
		if (path.empty())
		{
			std::filesystem::path localAppData = std::getenv("LOCALAPPDATA");
			std::filesystem::path minecraftPath = localAppData / "Packages" / "Microsoft.MinecraftWindowsBeta_8wekyb3d8bbwe" / "LocalState" / "games" / "com.mojang";
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
	const std::string& Addon::GetPreviewDevelopmentResourcePackPath()
	{
		static std::string path;
		if (path.empty())
		{
			std::filesystem::path localAppData = std::getenv("LOCALAPPDATA");
			std::filesystem::path minecraftPath = localAppData / "Packages" / "Microsoft.MinecraftWindowsBeta_8wekyb3d8bbwe" / "LocalState" / "games" / "com.mojang";
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
	NonOwningPtr<Addon> Addon::SetupStaticInstance(const Manifest& bpManifest,const Manifest& rpManifest, bool AutoRegisterBehAndResAsDeps)
	{
		std::println("Setting up static instance");
		addon = Addon(bpManifest, rpManifest, AutoRegisterBehAndResAsDeps);
		absl::InitializeLog();
		absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
		return &addon;
	}
	// The reason we have a singleton is because most of the time only a singel addon will be used but there maybe rare cases where more are needed
	// so we allow construction of multiple but the lifetime must be managed by the user


	NonOwningPtr<Addon> Addon::GetStaticInstance()
	{
		return &addon;
	}
	void Addon::generateBehaviourPack(const std::filesystem::path& outputPath, bool clearOutputFolder, bool cacheManifest)
	{
		std::filesystem::path path(outputPath);

		if (!std::filesystem::exists(path))
		{
			if (!std::filesystem::create_directory(path))
				throw std::runtime_error(std::format("Failed to create directory: {}",  outputPath.string()));
		}
		LOG(INFO) << "Writing blocks";
		writeBlocks(path);

		if (cacheManifest && std::filesystem::exists(path / "manifest.json"))
			return;
		LOG(INFO) << "Writing manifest";
		auto doc = mBehaviourPackManifest.WriteToDocument().value();
		std::filesystem::path manifestPath = path / "manifest.json";
		auto err = rapidjson::WriteJsonFile(doc, manifestPath);
		if (!err)
			throw std::runtime_error("Failed to write manifest: " + err.GetAsString());

	}
	void Addon::writeBlocks(const std::filesystem::path& outputBase)
	{
		std::filesystem::path blocksPath = outputBase;
		blocksPath /= "blocks";
		rapidjson::ForcePath(blocksPath);
		for (auto& [id, block] :mBlocks)
		{
			auto doc = block->WriteToDocument(this);
			std::filesystem::path blockPath = blocksPath / (block->GetIdentifier().GetFullNamespaceFile() + ".json");
			auto err = rapidjson::WriteJsonFile(doc, blockPath);
			if (!err)
				throw std::runtime_error("Failed to write block: " + err.GetAsString());
		}
	}

	void Addon::bindResource()
	{
		bindBlocks();
	}

	void Addon::bindBlocks()
	{
		for (auto& [id, block] : mBlocks)
		{
			bool hasBindableGeo = block->mGeo.GetGeoName() != "minecraft:geometry.full_block";
			if (block->mTextures.empty() && !hasBindableGeo)
				LOG(WARNING) << std::format("Block: {} does not have any textures", block->mIdentifier.GetFullNamespace());
			for (int i = 0; i < block->mTextures.size(); i++)
			{
				auto texture = std::move(block->mTextures[i]);
				if (texture.mTexture.has_value())
				{
					auto err = mTerrainTextureManager.BindBlockResourceElement(std::move(texture));
					if (err != Backend::Rp::TextureError::NONE)
						throw GetTextureErrorString(err);
				}
			}
			block->mTextures.clear();
			if (hasBindableGeo)
				mModelManager.AddModel(block->mGeo.GetGeoName(), block->mGeo);
		}
	}
	void Addon::generateResourcePack(const std::filesystem::path& outputPath, bool clearOutputFolder, bool cacheManifest)
	{
		LOG(INFO) << "Binding resources";
		bindResource();
		LOG(INFO) << "Bound resources";

		std::filesystem::create_directories(outputPath);
		LOG(INFO) << "Writing terrain textures";
		mTerrainTextureManager.setResourcePackName(mResourcePackManifest.GetName());
		writeTerrainTextures(outputPath);
		LOG(INFO) << "Writing models";
		writeModels(outputPath);
		if (cacheManifest && std::filesystem::exists(outputPath / "manifest.json"))
			return;
		LOG(INFO) << "Writing manifest";
		auto doc = mResourcePackManifest.WriteToDocument().value();
		std::filesystem::path manifestPath = outputPath / "manifest.json";
		auto err = rapidjson::WriteJsonFile(doc, manifestPath);
		if (!err)
			throw std::runtime_error("Failed to write manifest: " + err.GetAsString());
	}
	void Addon::writeTerrainTextures(const std::filesystem::path& outputBase)
	{
		auto err = mTerrainTextureManager.writeTextureData(outputBase);
		if (err.ContainsError())
		{
			std::println("Failed to write terrain textures: {}", err.GetAsString());
			throw std::runtime_error("Failed to write terrain textures");
		}
	}
	void Addon::writeModels(const std::filesystem::path& outputBase)
	{
	}
}
