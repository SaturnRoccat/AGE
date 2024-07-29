#pragma once
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>
#include <tuple>
#include <print>


namespace AgeAPI
{
	class Addon
	{
	public:
		Addon(
			Manifest&& bpManifest,
			Manifest&& rpManifest,
			bool AutoRegisterBehAndResAsDeps = true
		);
		template<typename T> requires std::constructible_from<Manifest, T&&>
		Addon(
			T&& bpManifest,
			T&& rpManifest,
			bool AutoRegisterBehAndResAsDeps = true
		) 
		{
			Addon(Manifest(std::forward<T>(bpManifest)), Manifest(std::forward<T>(rpManifest)), AutoRegisterBehAndResAsDeps);
		}

		void OutputAddon(const std::string& folderName, const std::pair<std::string, std::string>& outputPath = 
			{
				GetDevelopmentBehaviourPackPath(),
				GetDevelopmentResourcePackPath()
			}, bool ClearExistingData = true, bool cacheManifest = true);

		template<typename T>
		Addon& AddBlock(T&& block)
		{
			using NoRef = std::remove_reference_t<T>;
			mBlocks.push_back(std::make_unique<NoRef>(std::forward<T>(block)));
			return *this;
		}

		const std::string& GetName() const { return mName; }

		
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
		void generateBehaviourPack(const std::string& folderName, const std::string& outputPath, bool clearOutputFolder, bool cacheManifest);
	private:
		std::vector<std::unique_ptr<AddonFeatures::Block>> mBlocks{};
		std::string mName{};
		// The reason we hold *pointers* to the blocks is we want to allow some overwriting of block stuff 
		Manifest mBehaviourPackManifest{};
		Manifest mResourcePackManifest{};

	};
}