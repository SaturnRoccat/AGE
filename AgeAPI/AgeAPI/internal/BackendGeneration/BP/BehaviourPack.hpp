#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>

namespace AgeAPI::Backend::Bp
{
	class BehaviourPack
	{
	private:
		void writeBlocks(const std::filesystem::path& outputBase);
		void buildBehaviourPack(const std::filesystem::path& outputDir, bool makeManifest);
		void setManifest(Manifest& manifest) { mManifest = std::move(manifest); }
		BehaviourPack(NonOwningPtr<Addon> addon) : mAddon(addon){}
		friend class Addon;
	private:
		Manifest mManifest{};
		std::vector<BlockBehaviour> mBlockBehaviours{};
		NonOwningPtr<Addon> mAddon{};
	public:

		void WriteManifest(Manifest&& manifest) { mManifest = std::move(manifest); }
		const Manifest& GetManifest() const { return mManifest; }
		Manifest& GetManifest() { return mManifest; }
		const std::vector<BlockBehaviour>& GetBlockBehaviours() const { return mBlockBehaviours; }

		BehaviourPack& AddBlockBehaviour(BlockBehaviour&& blockBehaviour) { mBlockBehaviours.push_back(std::move(blockBehaviour)); return *this; }
		BehaviourPack& AddBlockBehaviour(BlockBehaviour& blockBehaviour) { mBlockBehaviours.push_back(std::move(blockBehaviour)); return  *this; }

		

	};
}