#pragma once
#include <AgeAPI/internal/Types.hpp>

namespace AgeAPI::Components
{

	class ComponentBase 
	{
	public:
		ComponentBase() = default;
		ComponentBase(const ExperimentalSettings& settings, const SemanticVersion& version, const Identifier& componentID)
			: mExperimentalSettings(settings), mFormatVersion(version), mComponentID(componentID) {}



		const ExperimentalSettings& GetExperimentalSettings() const { return mExperimentalSettings; }
		const SemanticVersion& GetFormatVersion() const { return mFormatVersion; }
		const Identifier& GetComponentID() const { return mComponentID; }

		void SetExperimentalSettings(const ExperimentalSettings& settings) { mExperimentalSettings = settings; }
		void SetFormatVersion(const SemanticVersion& version) { mFormatVersion = version; }
        void SetIdentifier(const Identifier& id) { mComponentID = id; }

	protected:
		ExperimentalSettings mExperimentalSettings;
		bool mCanBeDoublePushed = false; // This allows components to be pushed twice meaning if you did 
		//something like addcomponent twice it would not throw an error and just merge the two components
		SemanticVersion mFormatVersion;
		Identifier mComponentID;
	};

	template<typename T, typename U>
	concept Component = std::is_base_of_v<ComponentBase, U>;

}