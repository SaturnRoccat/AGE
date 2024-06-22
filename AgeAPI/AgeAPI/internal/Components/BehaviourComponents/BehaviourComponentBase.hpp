#pragma once 

#include <AgeAPI/internal/Components/ComponentBase.hpp>

namespace AgeAPI::Components
{
	class BehaviourComponentBase : public ComponentBase
	{
	public:
		BehaviourComponentBase() = default;
		BehaviourComponentBase(
			const ExperimentalSettings& settings,
			const SemanticVersion& version,
			const Identifier& ident,
			bool canBeDoublePushed = false,
			bool isTransient = false) : ComponentBase(settings, version, ident, canBeDoublePushed, isTransient) {}
		virtual ~BehaviourComponentBase() = default;
	};

    template<typename T, typename U>
	concept BehaviourComponent = std::is_base_of_v<BehaviourComponentBase, U>;
}