#pragma once

#include <AgeAPI/internal/Components/BehaviourComponents/BehaviourComponentBase.hpp>

namespace AgeAPI::Components
{
	class BlockComponentBase : public BehaviourComponentBase
	{
	public:
		BlockComponentBase() = default;
		BlockComponentBase(const ExperimentalSettings& settings, const SemanticVersion& version) : BehaviourComponentBase(settings, version) {}
		
		// This may not modify the component
		virtual void WriteToJson(Addon* addon, BlockJsonProxy& proxy) const = 0; 

	};
}
