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
			const Manifest& bpManifest = {},
			const Manifest& rpManifest = {},
			bool AutoRegisterBehAndResAsDeps = true
		);
		void OutputAddon(const std::string& folderName = "goobert", const std::pair<std::string, std::string>& outputPath =
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
			const Manifest& bpManifest = {},
			const Manifest& rpManifest = {},
			bool AutoRegisterBehAndResAsDeps = true);
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