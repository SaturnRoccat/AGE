#pragma once
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>
#include <tuple>
#include <absl/log/log.h>
#include <AgeAPI/internal/BackendGeneration/RP/TerrainTextures.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ModelManager.hpp>
#include <print>


namespace AgeAPI
{
	class Addon
	{
	public:
		Addon(
			const Manifest& bpManifest = {},
			const Manifest& rpManifest = {},
			bool AutoRegisterBehAndResAsDeps = true
		);
		void OutputAddon(bool cacheManifest = true, const std::string& folderName = "goobert", const std::pair<std::string, std::string>& outputPath =
			{
				GetDevelopmentBehaviourPackPath(),
				GetDevelopmentResourcePackPath()
			}, bool ClearExistingData = true);

		template<typename T>
		Addon& AddBlock(T&& block)
		{
			using NoRef = std::remove_reference_t<T>;
			mBlocks.push_back(std::make_unique<NoRef>(std::forward<T>(block)));
			return *this;
		}

		const std::string& GetName() const { return mName; }

		template<typename a1>
		void RegisterStandaloneBlockResource(a1&& resource, bool override = false)
		{
			auto it = mStandaloneResources.find(resource.mTextureAlias);
			if (!override && it != mStandaloneResources.end())
			{
				LOG(ERROR) << std::format("Resource with name {} already exists", resource.mTextureAlias);
				return;
			}
			mStandaloneResources[resource.mTextureAlias] = std::forward<a1>(resource);
		}

		
		static const std::string& GetDevelopmentBehaviourPackPath();
		static const std::string& GetDevelopmentResourcePackPath();
		static const std::string& GetPreviewDevelopmentBehaviourPackPath();
		static const std::string& GetPreviewDevelopmentResourcePackPath();
		static NonOwningPtr<Addon> SetupStaticInstance(
			const Manifest& bpManifest = {},
			const Manifest& rpManifest = {},
			bool AutoRegisterBehAndResAsDeps = true);
		static NonOwningPtr<Addon> GetStaticInstance();
	private:
		void generateBehaviourPack(const std::filesystem::path& outputPath, bool clearOutputFolder, bool cacheManifest);
		void writeBlocks(const std::filesystem::path& outputBase);
		void bindResource(); // Binds off all the stores of data we have to the internal representations of the data
		void bindBlocks();
		void generateResourcePack(const std::filesystem::path& outputPath, bool clearOutputFolder, bool cacheManifest);
		void writeTerrainTextures(const std::filesystem::path& outputBase);
		void writeModels(const std::filesystem::path& outputBase);

	private:
		std::vector<std::unique_ptr<AddonFeatures::Block>> mBlocks{};
		absl::flat_hash_map<std::string, Backend::Rp::BlockResourceElement> mStandaloneResources{};
		std::string mName{};
		// The reason we hold *pointers* to the blocks is we want to allow some overwriting of block stuff 
		Manifest mBehaviourPackManifest{};
		Manifest mResourcePackManifest{};
		Backend::Rp::TerrainTexture mTerrainTextureManager{};
		Backend::Rp::ModelManager mModelManager{};

	};
}