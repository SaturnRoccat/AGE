#pragma once
#include <AgeAPI/internal/Addon/AddonSpecifics/Manifest.hpp>
#include <AgeAPI/internal/BackendGeneration/BP/BehaviourPack.hpp>
#include <AgeAPI/internal/BackendGeneration/RP/ResourcePack.hpp>


namespace AgeAPI
{
	class Addon
	{
	public:
		Addon() {};
	private:
		Backend::Bp::BehaviourPack mBp{this};

	};
}