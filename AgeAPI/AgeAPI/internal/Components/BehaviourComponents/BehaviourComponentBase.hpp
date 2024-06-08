#pragma once 

#include <AgeAPI/internal/Components/ComponentBase.hpp>

namespace AgeAPI::Components
{
	class BehaviourComponentBase : public ComponentBase
	{
	public:
		BehaviourComponentBase() = default;
		BehaviourComponentBase(const ExperimentalSettings& settings, const SemanticVersion& version) : ComponentBase(settings, version) {}
	};
}