#pragma once
#include <AgeAPI/internal/BackendGeneration/BP/Block/BlockBehaviour.hpp>
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>

namespace AgeAPI::Backend::Bp
{
	class BehaviourPack
	{
	private:
		Manifest mManifest;
		std::vector<BlockBehaviour> mBlockBehaviours;

	};
}