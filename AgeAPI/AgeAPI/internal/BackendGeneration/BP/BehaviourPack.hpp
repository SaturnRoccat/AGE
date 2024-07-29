#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <AgeAPI/internal/Addon/AddonFeatures/Block.hpp>

namespace AgeAPI::Backend::Bp
{
	class BehaviourPack
	{
	public:

		Manifest& GetManifest() { return mManifest; }
		void WriteManifest(Manifest&& manifest) { mManifest = std::move(manifest); }
		const Manifest& GetManifest() const { return mManifest; }
		BehaviourPack(NonOwningPtr<Addon> addon) : mAddon(addon) {}
	private:
		void writeBlocks(const std::filesystem::path& outputBase);
		void buildBehaviourPack(const std::filesystem::path& outputDir, bool makeManifest);
		void forcePath(const std::filesystem::path& outputDir);
		void setManifest(Manifest&& manifest) { mManifest = std::move(manifest); }

		void setBlocks(std::vector<std::unique_ptr<AddonFeatures::Block>>&& blocks) { mBlocks = std::move(blocks); }
	private:
		Manifest mManifest{};
		std::vector<std::unique_ptr<AddonFeatures::Block>> mBlocks{};
		NonOwningPtr<Addon> mAddon{nullptr};
		friend class Addon;
	};
}