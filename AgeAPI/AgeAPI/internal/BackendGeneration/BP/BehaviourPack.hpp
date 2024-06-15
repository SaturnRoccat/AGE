#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>

namespace AgeAPI::Backend::Bp
{
	class BehaviourPack
	{
	private:
		Manifest mManifest{};
		std::vector<BlockBehaviour> mBlockBehaviours{};
	public:
		BehaviourPack() {};

		void WriteManifest(Manifest&& manifest) { mManifest = std::move(manifest); }
		const Manifest& GetManifest() const { return mManifest; }
		const std::vector<BlockBehaviour>& GetBlockBehaviours() const { return mBlockBehaviours; }

		void AddBlockBehaviour(BlockBehaviour&& blockBehaviour) { mBlockBehaviours.push_back(std::move(blockBehaviour)); }
		

	};
}