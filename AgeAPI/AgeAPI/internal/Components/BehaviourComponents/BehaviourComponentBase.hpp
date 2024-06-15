#pragma once 

#include <AgeAPI/internal/Components/ComponentBase.hpp>

namespace AgeAPI::Components
{
	class BehaviourComponentBase : public ComponentBase
	{
	public:
		BehaviourComponentBase() = default;
		BehaviourComponentBase(const ExperimentalSettings& settings, const SemanticVersion& version, const Identifier& ident) : ComponentBase(settings, version, ident) {}
		virtual ~BehaviourComponentBase() = default;
	};

    template<typename T, typename U>
	concept BehaviourComponent = std::is_base_of_v<BehaviourComponentBase, U>;
}